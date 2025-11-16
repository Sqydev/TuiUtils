TODO:
- Readme
- Name
- Do hidecursor && show cursor
- Make it so ClearChar and ClearLine takes an int HOW_MUCH argument
- FIX GETKEY AND KEYS MACROS
- Look for printfs that can be replaced with simple putchar
- End making it into 1 file
- Do CloseTui
- Make most #define to enums
- Make #ifdefs better especialy in GetKey()(cuz you can do only one in begining and not do 2 long switches for win and lin. just optim it)
- Clean .c to fit .h
- Make more Is fuctions, especialy for cursor functions for example: IsCursorHidden/Locked, Idea to how to find ones todo, look at bools in CORE
- Find better ways to do Move and Set Cursor fns, cuz there for sure is better way to do same thing that I did when i was tired, and check if it works
- FAST: GOTO DBP
- Rewrite tester after making begindrawing && end 
- MAKE GETKEY RETURN KEY THAT'S PRESSED NOT LAST KEY THAT'S PRESSED
- MAKE PRINTF INTO WRITEINTOBACKBUFFOR();
- Find what makes getkey and just LoopTest break(it's propably becouse most functions doesn't write to backbuffer yet(still do printf)) way to give vars to WritetoTui: {
    just make char* and do sprintf's faster brother's magic magic;
}
- CLEAN THIS MESS(Like here. in TODO.md)
- Make InitTui slimmer
- Just fix that fucking GetKey so it doesn't choak without skipping frame.
- Fix windows versions after and THEN make it slimmer
- FUCK
- In tester nr. 7 if terminal is too big cursor like's to go away for a seccond. BUT it SHOULD go away once I'll implement MoveCursor writing to BUFFOR not printf
- In the future, do mouse tracking things(like get mouse position)
- Remamber to do more get fns
- Find more fns that have same situation as HideCursor
- Find good name for WriteToBackBuffor
- Make sure that size of intbuffer in SecCursorPosition is max size of int. Try 23 in SetCursorPosition. Do the same for SetBackground And SetForegrnd 
- Do more get fns and use them in tester. (Like in HideCursor tester state of cursor hidness)
- REMAMBER TO CONVERT FROM PRITNF TO WRITETOBACKBUFFOR/write
- You forgot to make 'platform handling' and error handling in HideCursor and ShowCursor. Just search for write and look if these are done
- Jup, rectangle broke:(
- Ok, chainge the logic so backbuffor is array at dimmentions of the terminal. And every frame it just clear's WHOLE terminal and writes backbuffor. Becouse as I thouth the terminal is just retarted and you chan't thrust it with esqseq
- Jup, FOR SURE it's terminal's foult with thoes bug's that make's f.e. DRAWRECTANGLE not drawing full rectangle. In summ just do the thing up
- And backbuff is per cell. btw. esqseq like chainge color should still work. btw. delete styles(becouse utf-8)
- ....


TOUGHTS SAVE(In case cursor starts to flicker again(YES I FUCKING FIXED IT!!!!)):
- Never mind, flickering is still here. Fix it info: it hapens after about 5 sec and then idk, Idea. Do SetCursorPosition in EndDrawing but with write INSTEAD of WriteToBackBuffor, there's nothing left besides 1. terminal is retarded 2. It's floating point problem
- Backbuffor is ok, Cursor. is ok, I DON'T KNOW WHAT IT ISSSSS, I GIVE UP FOR NOW
- Mabey it's something with WriteToBackBuffor
- DrawRectangle works now but remamber that that was fixed by optimasing how much escseqs are spamed

REMAMBER:
- First make drawing fns and then fix things(But first first chainge printf to sprintf's faster brothers)
- DO DRAWING IN eshapes AND etext
- Remamber that in tester 7 cursor on top and getkey on bottom is intended, cuz Draw... will have where to draw it. SO DO NOT TRY TO FIX THIS BECOUSE IT IS JUST UNCLEAR INTENDED BEHAVIOUR
- If there is super big number of things in backbuffor there can be disturbences. It's only a theory thou so I'm leaving this note just in case it FLICKERING will happen again. Mabey thats becouse of that write instead of printf in EndDrawing
