This is the Chess Opening Learning Tool!

To compile the program, you need a C++ compiler (we use g++) and the raylib library, which can be found at https://www.raylib.com/. Once ready, run the following command from the project folder:

g++ src/*.cpp -o build/Chess_Opening_Learning_Tool.exe -O3 -IC:/raylib/raylib/src -LC:/raylib/raylib/src -lraylib -lopengl32 -lgdi32 -lwinmm

This will create the file "Chess_Opening_Learning_Tool.exe" in the build folder. IMPORTANT: Run the executable from the project folder via the following command:

./build/Chess_Opening_Learning_Tool.exe

If the program is ran in the build folder, it will not find the necessary files.

This repository has 5000 games as a sample because GitHub has a file size limit. If you want to initialize the full games database yourself, you need to have Python installed with the chess module. Download the games from https://database.nikonoel.fr/. Run the python file "src/pgn_parser.py" from the project folder and set the INPUT_PGN_FILE to the downloaded games file. It takes a few minutes to parse all the games. It will update you every 20,000 games parsed. 

In the program, you can toggle between Trie or Hash Map back-end via the button on the right. Left-click on a piece to select it, then left-click on another square to move it. You can choose pieces to place via the bar on the top. To go back to normal mode, press escape or select the same button on the bar again. Right-click to remove a piece. Left-click the White/Black to play button to change the current player.
On the right side, there is information stating how many games match the current position in the database and the proportion of them that ended with a white win, black win, and draw. There are 3 mini boards showing the 3 most popular next moves and their corresponding information.
