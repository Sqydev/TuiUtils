Basicly what things should do and how they work:

InitTui:
- atexit CloseTui
- if !DisableSignals make handler that triggers CloseTui at ctrl+c
- swap buffors
- then in #if do raw mode things and if DisableSignals then just add one flag to disable signals
- switch foreground and background colors without backbuff(just write)
