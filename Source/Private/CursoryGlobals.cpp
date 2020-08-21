// Copyright Mowgl33. All rights reserved.
#include "CursoryGlobals.h"
#include "Framework/Application/SlateApplication.h"
#include "GenericPlatform/ICursor.h"
#include "Misc/Paths.h"
#include "CursoryModule.h"
#include "Misc/CoreDelegates.h"
#include "IImageWrapperModule.h"
#include "HAL/FileManager.h"
#include "IImageWrapper.h"
#include "Misc/FileHelper.h"
#include "HAL/PlatformApplicationMisc.h"

#if WITH_EDITOR
#include "Editor.h"
#endif

#define LOCTEXT_NAMESPACE "CursoryGlobals"

UCursoryGlobals::UCursoryGlobals(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// Stub
}

void UCursoryGlobals::Init()
{
	static auto bInitialized = false;
	if (bInitialized)
	{
		UE_LOG(LogCursory, Error, TEXT("Tried to initialize CursoryGlobals after they were already initialized. Are you calling Init() twice?"));
	}

	// Load cursors once on Engine init.
	FCoreDelegates::OnPostEngineInit.AddWeakLambda(this, [this]()
	{
		LoadCursors();
	});

#if WITH_EDITOR

	// If in Editor, load cursors every time PIE starts.
	FEditorDelegates::BeginPIE.AddWeakLambda(this, [this](const bool bIsSimulating)
	{
		LoadCursors();
	});

#endif
}

struct FPNGConverter
{
	/** Describes PNG file data that can be used to
	*	create a cursor. */
	struct FPngFileData
	{
		FString FileName;
		double ScaleFactor;
		TArray<uint8> FileData;

		FPngFileData()
			: ScaleFactor(1.0)
		{
		}
	};

	static void* LoadCursorsFromPngs(ICursor& PlatformCursor, const FString& InPathToCursorWithoutExtension, FVector2D InHotSpot)
	{
		if (!PlatformCursor.IsCreateCursorFromRGBABufferSupported())
		{
			return nullptr;
		}

		TArray<TSharedPtr<FPngFileData>> cursorPngFiles;
		if (!LoadAvailableCursorPngs(cursorPngFiles, InPathToCursorWithoutExtension))
		{
			return nullptr;
		}

		check(cursorPngFiles.Num() > 0);
		TSharedPtr<FPngFileData> nearestCursor = cursorPngFiles[0];
		float platformScaleFactor = FPlatformApplicationMisc::GetDPIScaleFactorAtPoint(0, 0);
		for (TSharedPtr<FPngFileData>& fileData : cursorPngFiles)
		{
			const float newDelta = FMath::Abs(fileData->ScaleFactor - platformScaleFactor);
			if (newDelta < FMath::Abs(nearestCursor->ScaleFactor - platformScaleFactor))
			{
				nearestCursor = fileData;
			}
		}

		IImageWrapperModule& imageWrapperModule = FModuleManager::LoadModuleChecked<IImageWrapperModule>(FName("ImageWrapper"));
		TSharedPtr<IImageWrapper>pngImageWrapper = imageWrapperModule.CreateImageWrapper(EImageFormat::PNG);

		if (pngImageWrapper.IsValid() && pngImageWrapper->SetCompressed(nearestCursor->FileData.GetData(), nearestCursor->FileData.Num()))
		{
			TArray64<uint8> rawImageData;
			if (pngImageWrapper->GetRaw(ERGBFormat::RGBA, 8, rawImageData))
			{
				const int32 width = pngImageWrapper->GetWidth();
				const int32 height = pngImageWrapper->GetHeight();

				return PlatformCursor.CreateCursorFromRGBABuffer((FColor*)rawImageData.GetData(), width, height, InHotSpot);
			}
		}

		return nullptr;
	}

	static bool LoadAvailableCursorPngs(TArray<TSharedPtr<FPngFileData>>& Results, const FString& InPathToCursorWithoutExtension)
	{
		FString cursorsWithSizeSearch = FPaths::GetCleanFilename(InPathToCursorWithoutExtension) + TEXT("*.png");

		TArray<FString> pngCursorFiles;
		IFileManager::Get().FindFilesRecursive(pngCursorFiles, *FPaths::GetPath(InPathToCursorWithoutExtension), *cursorsWithSizeSearch, true, false, false);

		bool bFoundCursor = false;

		for (const FString& fullCursorPath : pngCursorFiles)
		{
			FString cursorFile = FPaths::GetBaseFilename(fullCursorPath);

			FString dummy;
			FString scaleFactorSection;
			FString scaleFactor;

			if (cursorFile.Split(TEXT("@"), &dummy, &scaleFactorSection, ESearchCase::IgnoreCase, ESearchDir::FromEnd))
			{
				if (scaleFactorSection.Split(TEXT("x"), &scaleFactor, &dummy) == false)
				{
					scaleFactor = scaleFactorSection;
				}
			}
			else
			{
				scaleFactor = TEXT("1");
			}

			if (FCString::IsNumeric(*scaleFactor) == false)
			{
				UE_LOG(LogInit, Error, TEXT("Failed to load cursor '%s', non-numeric characters in the scale factor."), *fullCursorPath);
				continue;
			}

			TSharedPtr<FPngFileData> pngFileData = MakeShared<FPngFileData>();
			pngFileData->FileName = fullCursorPath;
			pngFileData->ScaleFactor = FCString::Atof(*scaleFactor);

			if (FFileHelper::LoadFileToArray(pngFileData->FileData, *fullCursorPath, FILEREAD_Silent))
			{
				UE_LOG(LogInit, Log, TEXT("Loading Cursor '%s'."), *fullCursorPath);
			}

			Results.Add(pngFileData);

			bFoundCursor = true;
		}

		Results.StableSort([](const TSharedPtr<FPngFileData>& InFirst, const TSharedPtr<FPngFileData>& InSecond) -> bool
		{
			return InFirst->ScaleFactor < InSecond->ScaleFactor;
		});

		return bFoundCursor;
	}
};

void UCursoryGlobals::LoadCursors()
{
	LoadedCursors.Reset();

	TSharedPtr<ICursor> platformCursor = FSlateApplication::Get().GetPlatformCursor();

	// Iterate through specs and load cursor handles.
	for (auto& cur : CursorSpecs)
	{
		// Validate hot spot.
		auto& hotspot = cur.Hotspot;
		ensure(hotspot.X >= 0.0f && hotspot.X <= 1.0f);
		ensure(hotspot.Y >= 0.0f && hotspot.Y <= 1.0f);
		hotspot.X = FMath::Clamp(hotspot.X, 0.0f, 1.0f);
		hotspot.Y = FMath::Clamp(hotspot.Y, 0.0f, 1.0f);

		// Create the cursor on the platform.
		FString cursorFullPath = FPaths::ProjectContentDir() / cur.Path;
		auto hardwareCursor = platformCursor->CreateCursorFromFile(cursorFullPath, hotspot);
		if (!hardwareCursor)
		{
			// Fallback to PNG loading.
			hardwareCursor = FPNGConverter::LoadCursorsFromPngs(*platformCursor, cursorFullPath, hotspot);
			if (!hardwareCursor)
			{
				UE_LOG(LogCursory, Warning, TEXT("Failed to load hardware cursor [%s] located at [%s]."), *cur.Identifier.ToString(), *cursorFullPath);
				continue;
			}
		}

		// Save cursor handle.
		LoadedCursors.Add(cur.Identifier, hardwareCursor);
	}
}

int32 UCursoryGlobals::GetCustomCursorCount() const
{
	return LoadedCursors.Num();
}

FGameplayTagContainer UCursoryGlobals::GetCustomCursorOptions() const
{
	auto options = FGameplayTagContainer();
	for (const auto& loadedCursor : LoadedCursors) 
	{
		options.AddTag(loadedCursor.Key);
	}
	return options;
}

FGameplayTag UCursoryGlobals::GetMountedCustomCursor() const
{
	return MountedCursor;
}

void UCursoryGlobals::MountCustomCursor(FGameplayTag& Identifier)
{
	if (auto cursor = LoadedCursors.Find(Identifier))
	{
		MountedCursor = Identifier;
		FSlateApplication::Get().GetPlatformCursor()->SetTypeShape(EMouseCursor::Custom, *cursor);
	}

	else
	{
		UE_LOG(LogCursory, Warning, TEXT("Tried to mount custom cursor [%s], but no such cursor has been loaded."), *Identifier.ToString());
	}
}

#undef LOCTEXT_NAMESPACE