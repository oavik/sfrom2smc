This program extracts raw SMC ROMs from Nintendo's proprietary SFROM format, used
by at least the Super NES classic.

To compile, just run `make` in this directory.

To convert a ROM, run:

    sfrom2smc my-game.sfrom

This will create `my-game.smc`.

Or run:

    sfrom2smc my-game.sfrom a-new-name.smc

That's it. You should now be able to use the SMC ROM in your emulator of choice.
