This is the Chess Opening Learning Tool!

To compile the program, you need a C++ compiler (we use g++) and the raylib library, which can be found at https://www.raylib.com/. Once ready, run the following command from the project folder:

g++ src/*.cpp -o build/Chess_Opening_Learning_Tool.exe -O3 -IC:/raylib/raylib/src -LC:/raylib/raylib/src -lraylib -lopengl32 -lgdi32 -lwinmm

This will create the file "Chess_Opening_Learning_Tool.exe" in the build folder. IMPORTANT: Run the executable from the project folder via the following command:

./build/Chess_Opening_Learning_Tool.exe

If the program is ran in the build folder, it will not find the necessary files.

In the program, you can use either the Trie or Hash Map back-end. Left-click on a piece to select it, then left-click on another square to move it. You can choose pieces to place via the bar on the top. To go back to normal mode, press escape or select the same button on the bar again. Right-click to remove a piece. 
On the right side, there is information stating how many games match the current position in the database and the proportion of them that ended with a white win, black win, and draw. There are 3 mini boards showing the 3 most popular next moves and their corresponding information.
