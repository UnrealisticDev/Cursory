# Cursory

Cursory is a simple plugin for Unreal Engine 4 that enables developers to use hardware cursors more easily in their projects. It does this by 1) enabling organization and 2) increasing ease of use.

## Organization

Hardware cursors can be defined at development time in Project Settings. Further, they are identified and organized by GameplayTags. For example, if you had a grab mechanic in your game, you could have three different GameplayTag identifiers for different cursors, depending on the state of the grabbable item:

1. Cursors.Pickup.Ungrabbable
2. Cursors.Pickup.Grabbable
3. Cursors.Pickup.Grabbed

Significantly, custom defined cursors *do not* override standard system cursors. This means you can define as many custom cursors as you want at development time while maintaing access to system cursors like the text edit beam, hourglass, and others. 

## Ease of Use

Hardware cursors can be used in game (through C++ and Blueprints) by calling two simple functions: `UseCustomCursor` and `UseStandardCursor`. The first function takes a GameplayTag as a parameter - you should pass the GameplayTag for one of the cursors defined in your Project Settings. The second function takes a MouseCursor enum parameter, which is defined in the Engine and which allows you to pick from the various system cursors.

Together, organization and ease of use make using hardware cursors a pleasant developer experience.
