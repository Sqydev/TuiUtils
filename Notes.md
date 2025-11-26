1. Remove SetForegroundColor and like that
2. EscSeq for colors are safe SO I can make backbuff for whole screen and every cell has escseq for color if needed and it should work
3. All operations like SetCursorPos are only chinging var and are applied by EndDrawing as escseq
4. Remamber to setup foregroundStyle, Color and Bg_color
5. Remamber to make DrawX get argument in form of int that can be ASCII or UTF-8 and draw them respectivly or smf
6. Remamber that it's the terminal's fould, you can't make it as good as gui. But with diffing unless all of the 'pixels' are chainging it should be ok
7. Remamber to give info for the dev's using it that they should prpbl give info for users of their thing that the user should use sum terminal with gpu for better performence and how bit the gap is
