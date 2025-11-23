#pragma once
#include "utils/stb_image.h"
#include <vector>

namespace SpaceEngine{

    enum class TitleResult {
        NONE,           // Nessuna azione (l'utente sta solo muovendo il mouse)
        PLAY,           // Ha cliccato "Nuova Partita"
        OPTIONS,        // Ha cliccato "Opzioni"
        LEADERBOARD,    // Ha cliccato "Classifica"
        EXIT            // Ha cliccato "Esci"
    };

    // --- Struttura del bottone (che usiamo internamente) ---
    struct MenuButton {
        float x, y;
        float w, h;
        unsigned int textureID;
        // unsigned int textureHoverID, se vogliamo cambiare texture al passaggio del mouse
        TitleResult action;
        bool isHovered;
    };

    class TitleScreen{
    public:
        TitleScreen();
        ~TitleScreen();

        void Init();
        void Render();
        TitleResult getInput();

    private:
        bool StartNewGame(); //bool per avere conferma che la funzione sia stata lanciata correttamente
        bool OpenOptions();
        bool OpenLeaderboard();
        bool ExitGame();
        float logoX, logoY;
        float logoW, logoH; //TODO: da settare in Init in base alla dimensione della texture
        unsigned int texStart, texOptions, texLeaderboard, texExit, texLogo;
        
        std::vector<MenuButton> m_buttons;
    };
}