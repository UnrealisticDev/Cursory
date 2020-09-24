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
		if (FSlateApplication::IsInitialized())
		{
			LoadCursors();
		}
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
	/** 
	 * Describes PNG file data that can be used to
	 * create a cursor. 
	 */
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

		TArray<TSharedPtr<FPngFileData>> CursorPngFiles;
		if (!LoadAvailableCursorPngs(CursorPngFiles, InPathToCursorWithoutExtension))
		{
			return nullptr;
		}

		check(CursorPngFiles.Num() > 0);
		TSharedPtr<FPngFileData> NearestCursor = CursorPngFiles[0];
		float PlatformScaleFactor = FPlatformApplicationMisc::GetDPIScaleFactorAtPoint(0, 0);
		for (TSharedPtr<FPngFileData>& FileData : CursorPngFiles)
		{
			const float NewDelta = FMath::Abs(FileData->ScaleFactor - PlatformScaleFactor);
			if (NewDelta < FMath::Abs(NearestCursor->ScaleFactor - PlatformScaleFactor))
			{
				NearestCursor = FileData;
			}
		}

		IImageWrapperModule& ImageWrapperModule = FModuleManager::LoadModuleChecked<IImageWrapperModule>(FName("ImageWrapper"));
		TSharedPtr<IImageWrapper>PngImageWrapper = ImageWrapperModule.CreateImageWrapper(EImageFormat::PNG);

		if (PngImageWrapper.IsValid() && PngImageWrapper->SetCompressed(NearestCursor->FileData.GetData(), NearestCursor->FileData.Num()))
		{
			TArray64<uint8> RawImageData;
			if (PngImageWrapper->GetRaw(ERGBFormat::RGBA, 8, RawImageData))
			{
				const int32 Width = PngImageWrapper->GetWidth();
				const int32 Height = PngImageWrapper->GetHeight();

				return PlatformCursor.CreateCursorFromRGBABuffer((FColor*)RawImageData.GetData(), Width, Height, InHotSpot);
			}
		}

		return nullptr;
	}

	static bool LoadAvailableCursorPngs(TArray<TSharedPtr<FPngFileData>>& Results, const FString& InPathToCursorWithoutExtension)
	{
		FString CursorsWithSizeSearch = FPaths::GetCleanFilename(InPathToCursorWithoutExtension) + TEXT("*.png");

		TArray<FString> PngCursorFiles;
		IFileManager::Get().FindFilesRecursive(PngCursorFiles, *FPaths::GetPath(InPathToCursorWithoutExtension), *CursorsWithSizeSearch, true, false, false);

		bool bFoundCursor = false;

		for (const FString& FullCursorPath : PngCursorFiles)
		{
			FString CursorFile = FPaths::GetBaseFilename(FullCursorPath);

			FString Dummy;
			FString ScaleFactorSection;
			FString ScaleFactor;

			if (CursorFile.Split(TEXT("@"), &Dummy, &ScaleFactorSection, ESearchCase::IgnoreCase, ESearchDir::FromEnd))
			{
				if (ScaleFactorSection.Split(TEXT("x"), &ScaleFactor, &Dummy) == false)
				{
					ScaleFactor = ScaleFactorSection;
				}
			}
			else
			{
				ScaleFactor = TEXT("1");
			}

			if (FCString::IsNumeric(*ScaleFactor) == false)
			{
				UE_LOG(LogInit, Error, TEXT("Failed to load cursor '%s', non-numeric characters in the scale factor."), *FullCursorPath);
				continue;
			}

			TSharedPtr<FPngFileData> PngFileData = MakeShared<FPngFileData>();
			PngFileData->FileName = FullCursorPath;
			PngFileData->ScaleFactor = FCString::Atof(*ScaleFactor);

			if (FFileHelper::LoadFileToArray(PngFileData->FileData, *FullCursorPath, FILEREAD_Silent))
			{
				UE_LOG(LogInit, Log, TEXT("Loading Cursor '%s'."), *FullCursorPath);
			}

			Results.Add(PngFileData);

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

	TSharedPtr<ICursor> PlatformCursor = FSlateApplication::Get().GetPlatformCursor();

	// Iterate through specs and load cursor handles.
	for (FCursorInfo& CursorSpec : CursorSpecs)
	{
		// Validate hot spot.
		FVector2D& Hotspot = CursorSpec.Hotspot;
		ensure(Hotspot.X >= 0.0f && Hotspot.X <= 1.0f);
		ensure(Hotspot.Y >= 0.0f && Hotspot.Y <= 1.0f);
		Hotspot.X = FMath::Clamp(Hotspot.X, 0.0f, 1.0f);
		Hotspot.Y = FMath::Clamp(Hotspot.Y, 0.0f, 1.0f);

		// Create the cursor on the platform.
		FString CursorFullPath = FPaths::ProjectContentDir() / CursorSpec.Path;
		void* HardwareCursor = PlatformCursor->CreateCursorFromFile(CursorFullPath, Hotspot);
		if (!HardwareCursor)
		{
			// Fallback to PNG loading.
			HardwareCursor = FPNGConverter::LoadCursorsFromPngs(*PlatformCursor, CursorFullPath, Hotspot);
			if (!HardwareCursor)
			{
				UE_LOG(LogCursory, Warning, TEXT("Failed to load hardware cursor [%s] located at [%s]."), *CursorSpec.Identifier.ToString(), *CursorFullPath);
				continue;
			}
		}

		// Save cursor handle.
		LoadedCursors.Add(CursorSpec.Identifier, HardwareCursor);
	}
}

int32 UCursoryGlobals::GetCustomCursorCount() const
{
	return LoadedCursors.Num();
}

FGameplayTagContainer UCursoryGlobals::GetCustomCursorOptions() const
{
	FGameplayTagContainer Options;
	for (const auto& LoadedCursorPair : LoadedCursors) 
	{
		Options.AddTag(LoadedCursorPair.Key);
	}
	return Options;
}

FGameplayTag UCursoryGlobals::GetMountedCustomCursor() const
{
	return MountedCursor;
}

void UCursoryGlobals::MountCustomCursor(FGameplayTag& Identifier)
{
	if (auto Cursor = LoadedCursors.Find(Identifier))
	{
		MountedCursor = Identifier;
		FSlateApplication::Get().GetPlatformCursor()->SetTypeShape(EMouseCursor::Custom, *Cursor);
	}

	else
	{
		UE_LOG(LogCursory, Warning, TEXT("Tried to mount custom cursor [%s], but no such cursor has been loaded."), *Identifier.ToString());
	}
}

#undef LOCTEXT_NAMESPACE