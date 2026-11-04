# Chess

A chess game written in C++ using [raylib](https://www.raylib.com/).

<img width="607" height="610" alt="image" src="https://github.com/user-attachments/assets/7c6ea802-b8f3-4aa8-aef0-67e0159f9752" />

## Features

- 600x600 board rendered in an 800x800 window, with piece sprites loaded from a sprite sheet
- Legal move validation, including filtering out moves that leave your own king in check
- Check and checkmate detection
- Special moves: castling, en passant, and pawn promotion
- Positions can be loaded from [FEN](https://en.wikipedia.org/wiki/Forsyth%E2%80%93Edwards_Notation) strings, which makes it easy to test edge cases
- Debug build mode (`DEBUG_MODE`) with extra console output

## Requirements

- Linux with X11 and OpenGL (the makefile links `-lGL` and `-lX11`)
- `g++` and `make`
- [raylib](https://github.com/raysan5/raylib) built and installed so that `-lraylib` resolves

## Building and Running

Executables are placed in the `build/` folder. Run them from the project root so the game can find the `resources/` folder.

| Command | What it does |
| --- | --- |
| `make release` | Builds an optimized binary at `build/release` |
| `make debug` | Builds an optimized binary with debug symbols at `build/debug` |
| `make debug_no_o` | Builds with debug symbols and no optimization |
| `make build/test` | Builds the test program at `build/test` |
| `make run_release` | Builds (if needed) and runs the release binary |
| `make run_debug` (or `make run`) | Builds (if needed) and runs the debug binary |
| `make run_test` | Builds (if needed) and runs the tests |
| `make clean` | Deletes everything in `build/` |

## Testing Positions

The game starts from a FEN string passed to the `ChessGame` constructor in `main.cpp`. To test a specific situation, change that string. Example positions for castling, en passant, check, checkmate, and promotion are included as comments in `main.cpp`, and more are in `resources/FENstrings.txt`.

```cpp
ChessGame game({100, 100}, 600, "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
```

## How to Play

The game uses click-to-move controls:

1. Click one of your pieces to select it.
2. Click a destination square to move it there.

## Project Structure

```
.
├── main.cpp          # Window setup and main loop
├── game.h            # ChessGame class
├── attributes.h
├── interactions.h
├── position.h
├── states.h
├── textures.h
├── testing.cpp       # Test harness
├── resources/
│   ├── chesspieces.png   # Piece sprite sheet
│   └── FENstrings.txt    # Sample FEN positions
├── makefile
└── idea.txt
```

## Credits and Acknowledgments

- **Library:** [raylib](https://www.raylib.com/) by Ramon Santamaria (@raysan5), licensed under the zlib/libpng license.
- **Piece sprites:** `resources/chesspieces.png` is from [HiClipart](https://www.hiclipart.com/free-transparent-background-png-clipart-qnbrg), an open community for sharing PNG images. HiClipart states that its images are for non-commercial use only and require no attribution. The original author is not identified.
- **AI assistance:** This README was written with the help of [Claude](https://www.anthropic.com/claude) (Claude Sonnet 5.5), an AI assistant made by Anthropic, in November 2026.


## License

The source code is licensed under the MIT License. See `LICENSE` for details.

The piece sprites (`resources/chesspieces.png`) are not covered by this license. They are limited to non-commercial use, as described in the credits above.
