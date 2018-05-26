[![Build status](https://ci.appveyor.com/api/projects/status/3v6dhp9rxwn9qu80/branch/master?svg=true)](https://ci.appveyor.com/project/paulsapps/msgi/branch/master)

### FAQ:

#### What?

This project aims to be a full source reverse engineering project of Metal Gear Solid Integral PC. This means "converting" all of the MGSI.EXE to recompilable source code.

#### How?

By using a DLL which the game loads it is then possible to replace functions at run time. The game already loads a DLL of its own free will. Therefore this DLL can simply be replaced with code which takes over the WinMain entry point.

E.g:

1. MGSI.EXE starts up. 
2. Its WinMain function does a LoadLibrary on LibEULA.dll in order to display the EULA dialog.
3. Since this DLL has been replaced with our code we now have a DLL loaded and executing code in the content of MGSI.EXE
4. From here we can execute our replacement WinMain (without the LibEULA.dll loading part to avoid recursion). And hook all functions that have been re-implemented.

So for example:

```
void CC Actor_DestroyOnNextUpdate(Actor* pActor)
{
    pActor->mFnUpdate = Actor_Destroy;
}
MGS_FUNC_IMPLEX(0x0040A3ED, Actor_DestroyOnNextUpdate, true);
```

Will cause the function in MGSI.EXE at address 0x0040A3ED to call the replacement function Actor_DestroyOnNextUpdate. This allows verification of this single function without having to reimplement everything up to its call point.

#### Why?

- Knowledge.
- Mods (knowing the games file formats and workings inside out will unlock this, adding any required features will be possible).
- Compatiblity/fixes (legacy API's can be removed/replaced such as using DirectInput or DirectX7).
- Portablity (when complete you can port to Linux,OSX,Android,XYZ).
- Features (controller vibration can be restored, fixing the PSX graphics emulation so Z-Fighting/depth issues are resolved).

#### What do I need/how do I contribute?

- A full copy of Metal Gear Solid Integral PC.
- Knowledge of C/C++ and x86 assembly.
- A copy of the latest IDA database. 
- Visual Studio (will change in the future when the game is fully reversed and works standalone, for now the MSVC ABI is required for replacement functions to be compatible. Addtionally the community/free editions are fine).
- Be crazy enough to take on a project like this.

To get a feel for what is required a good first task will be to pick a simple unreversed function. Understand what it does and send in a PR to implement it.

#### Any IRC/Discord etc?

Discord here: https://discord.gg/tTvhQ8w
