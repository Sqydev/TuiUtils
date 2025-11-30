TODO:
- Do backbuff like in 2 in Notes
- Do universal write fn(for lazyness)
- Segregate thoes fns(in .c and .h)
- Find better name for ShouldHideCursor in InitTui
- Think about: Should There be different SetCursorPos && SetLockedCursorPos, cuz if cursor is locked than SetCursorPos just chainges lockedcursorpos. And that is how other's work
- Check if resize recognition works in DisableStandardSIgs == true
- Consider doing utils.c for things with static prefix
- Make another delta time that measures drawing delta time. starts at the end of begin drawing
- When BeginDrawing && EndDrawing works make BeginDrawing(Name of this rendering that renders only the chainged cells) and same with end
- Make sure thet signal handling clears the other jobs
- Make termcurpos uint
- Check if U_INT IS PLATFORM DEPENDENT!!!!!!
- Rewrite EndDrawing from chatgbt
- Make thing like loading screen fn.
- Fix the problem that in the test 6 it prints only one 😀(It's becouse 😀 takes place of 2 chars so it breaks or smf)


TOUGHTS SAVE(In case cursor starts to flicker again(YES I FUCKING FIXED IT!!!!)):
- Never mind, flickering is still here. Fix it info: it hapens after about 5 sec and then idk, Idea. Do SetCursorPosition in EndDrawing but with write INSTEAD of WriteToBackBuffor, there's nothing left besides 1. terminal is retarded 2. It's floating point problem
- Backbuffor is ok, Cursor. is ok, I DON'T KNOW WHAT IT ISSSSS, I GIVE UP FOR NOW
- Mabey it's something with WriteToBackBuffor
- DrawRectangle works now but remamber that that was fixed by optimasing how much escseqs are spamed

REMAMBER:
- Remamber that in tester 7 cursor on top and getkey on bottom is intended, cuz Draw... will have where to draw it. SO DO NOT TRY TO FIX THIS BECOUSE IT IS JUST UNCLEAR INTENDED BEHAVIOUR
- If there is super big number of things in backbuffor there can be disturbences. It's only a theory thou so I'm leaving this note just in case it FLICKERING will happen again. Mabey thats becouse of that write instead of printf in EndDrawing
