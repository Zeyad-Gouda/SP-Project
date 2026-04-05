// ============================================================
// FEEDING FRENZY 2 - MERGED MAIN FILE
// Parts: Loading Screen (Doc1) + Main Menu (Doc2) + Switch User (Doc3)
// ============================================================

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Main.hpp>
#include <SFML/Network.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <optional>
#include <cstdint>
#include <cctype>
#include <array>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <random>
#include <fstream>

using namespace std;
using namespace sf;

// ============================================================
// --- SHARED WINDOW & VIEW (Doc1 window + Doc2 win + Doc3 window → merged) ---
// Doc1 was 800x600 logical fullscreen, Doc2 was 1280x720 windowed, Doc3 was 1920x1080
// Resolution unified to 1920x1080 fullscreen as agreed
// ============================================================
float WindowWidth = 1920;
float WindowHeight = 1080;
RenderWindow window(VideoMode({(unsigned int)WindowWidth, (unsigned int)WindowHeight}), "Feeding Frenzy 2", State::Fullscreen);
View view({WindowWidth / 2.f, WindowHeight / 2.f}, Vector2f(window.getSize()));

// deltaTime — unified, calculated once in main loop
float deltaTime = 0.f;

// ============================================================
// ==================== LOADING SCREEN (Doc1) =================
// ============================================================

Shader swayShader;
float startX_loading = WindowWidth / 2.f;
float spacing = 65.0f;
float width_bubble_global;
float endTimer = 0.0f;
bool transitionStarted = false;

Vector2f ventPositions[2] = {
    {WindowWidth - 140.f, WindowHeight - 157.5f},
    {WindowWidth - 220.f, WindowHeight - 117.f}};

struct Particle
{
    float x, y, vx, vy;
    float alpha = 255.0f;
    bool active = false;
};

struct AmbientBubble
{
    float x, y, vx, vy;
    float alpha = 255.0f;
    bool active = false;
    bool isVent = false;
};

AmbientBubble ventBubbles[60];
float bubbleTimer = 0.0f;

Particle sparks[35];
Particle letterBubbles[35];
bool exploded = false;
bool particlesFinished = false;
float timerLeft = 0.0f;
float timerRight = 0.0f;

Texture Background_Loading;
Sprite Background_Loading_sprite(Background_Loading);
Texture Game_Icon;
Sprite Game_Icon_sprite(Game_Icon);
Texture POPCAP_Logo;
Sprite POPCAP_Logo_sprite(POPCAP_Logo);
Texture LogoWood;
Sprite LogoWood_sprite(LogoWood);
Texture loadBar;
Sprite loadBar_sprite(loadBar);
Texture loadBar_empty;
Sprite loadBar_empty_sprite(loadBar_empty);
Texture bubbleTexture;
Sprite bubbleSprite(bubbleTexture);
Texture fontTexture;
Sprite s_L(fontTexture);
Sprite s_O(fontTexture);
Sprite s_A(fontTexture);
Sprite s_D(fontTexture);
Sprite s_I(fontTexture);
Sprite s_N(fontTexture);
Sprite s_G(fontTexture);
Texture bubbleSmallTex;
Sprite bubbleSmallSprite(bubbleSmallTex);
Texture spark;
Sprite sparkSprite(spark);

void loading_screen_init()
{
    window.setFramerateLimit(45);

    if (!swayShader.loadFromFile("sway.frag", Shader::Type::Fragment))
        cout << "Error loading shader!" << endl;

    if (!Background_Loading.loadFromFile("Assets/Main menu & Loading/Loading Screen/Images/loadbg.png"))
        cout << "!!! Error: Could not find loadbg.png !!!" << endl;
    Background_Loading_sprite.setTexture(Background_Loading);
    Background_Loading_sprite.setPosition({0, 0});
    Vector2u texture_Size = Background_Loading.getSize();
    float ScaleX = WindowWidth / texture_Size.x;
    float ScaleY = WindowHeight / texture_Size.y;
    Background_Loading_sprite.setScale({ScaleX, ScaleY});
    Background_Loading.setSmooth(true);

    if (!Game_Icon.loadFromFile("Assets/Main menu & Loading/Loading Screen/Images/LOGO.png"))
        cout << "!!! Error: Could not find LOGO.png !!!" << endl;
    float width = 300, height = 150;
    Game_Icon_sprite.setTexture(Game_Icon);
    Game_Icon_sprite.setPosition({(WindowWidth / 2) - 155, 7});
    ScaleX = width / Game_Icon.getSize().x;
    ScaleY = height / Game_Icon.getSize().y;
    Game_Icon_sprite.setScale({ScaleX, ScaleY});
    Game_Icon.setSmooth(true);

    if (!POPCAP_Logo.loadFromFile("Assets/Main menu & Loading/Loading Screen/Images/popCapLogo.png"))
        cout << "!!! Error: Could not find popCapLogo.png !!!" << endl;
    width = 120;
    height = 110;
    POPCAP_Logo_sprite.setTexture(POPCAP_Logo);
    POPCAP_Logo_sprite.setPosition({(WindowWidth - 155), WindowHeight - 126.f});
    ScaleX = width / POPCAP_Logo.getSize().x;
    ScaleY = height / POPCAP_Logo.getSize().y;
    POPCAP_Logo_sprite.setScale({ScaleX, ScaleY});
    POPCAP_Logo.setSmooth(true);

    if (!LogoWood.loadFromFile("Assets/Main menu & Loading/Loading Screen/Images/sproutLogoWood.png"))
        cout << "!!! Error: Could not find sproutLogoWood.png !!!" << endl;
    width = 160;
    height = 80;
    LogoWood_sprite.setTexture(LogoWood);
    LogoWood_sprite.setPosition({(0 + 25), WindowHeight - 115.f});
    ScaleX = width / LogoWood.getSize().x;
    ScaleY = height / LogoWood.getSize().y;
    LogoWood_sprite.setScale({ScaleX, ScaleY});
    LogoWood.setSmooth(true);

    if (!fontTexture.loadFromFile("Assets/Fonts/LOADING.png"))
        cout << "!!! Error: Could not find LOADING.png !!!" << endl;
    fontTexture.setSmooth(true);

    float cellWidth = 19.4f;
    float cellHeight = 35.5f;
    Color targetColor(160, 211, 74);

    s_L.setTexture(fontTexture);
    s_L.setTextureRect(IntRect({(int)(1 * cellWidth), (int)(3 * cellHeight)}, {(int)cellWidth, (int)cellHeight}));
    s_L.setColor(targetColor);
    s_L.setOrigin({cellWidth / 2.f, cellHeight / 2.f});

    s_O.setTexture(fontTexture);
    s_O.setTextureRect(IntRect({(int)(4 * 20), (int)(3 * 35.7)}, {20, (int)35.7}));
    s_O.setColor(targetColor);
    s_O.setOrigin({10.f, 35.7f / 2.f});

    s_A.setTexture(fontTexture);
    s_A.setTextureRect(IntRect({(int)(3 * 20), (int)(2 * cellHeight)}, {20, (int)cellHeight}));
    s_A.setColor(targetColor);
    s_A.setOrigin({10.f, cellHeight / 2.f});

    s_D.setTexture(fontTexture);
    s_D.setTextureRect(IntRect({(int)(6 * 20), (int)(2 * cellHeight)}, {20, (int)cellHeight}));
    s_D.setColor(targetColor);
    s_D.setOrigin({10.f, cellHeight / 2.f});

    s_I.setTexture(fontTexture);
    s_I.setTextureRect(IntRect({(int)(10.39 * 20 + 4), (int)(2 * cellHeight)}, {12, (int)cellHeight}));
    s_I.setColor(targetColor);
    s_I.setOrigin({6.f, cellHeight / 2.f});

    s_N.setTexture(fontTexture);
    s_N.setTextureRect(IntRect({(int)(3 * 20), (int)(3 * cellHeight)}, {20, (int)cellHeight}));
    s_N.setColor(targetColor);
    s_N.setOrigin({10.f, cellHeight / 2.f});

    s_G.setTexture(fontTexture);
    s_G.setTextureRect(IntRect({174, (int)(2 * cellHeight)}, {18, (int)cellHeight}));
    s_G.setColor(targetColor);
    s_G.setOrigin({9.f, cellHeight / 2.f});

    bubbleSprite.setTexture(bubbleTexture);
    if (!bubbleTexture.loadFromFile("Assets/Main menu & Loading/Loading Screen/Images/bubbles_letters.png"))
        cout << "!!! Error: Could not find bubbles_letters.png !!!" << endl;
    bubbleSprite.setTexture(bubbleTexture);
    bubbleSprite.setScale({0.7f, 0.7f});
    bubbleSprite.setOrigin({bubbleSprite.getLocalBounds().size.x / 2.f, bubbleSprite.getLocalBounds().size.y / 2.f});
    width_bubble_global = bubbleSprite.getGlobalBounds().size.x;
    bubbleTexture.setSmooth(true);

    width = 320.f;
    height = 50.f;
    if (!loadBar_empty.loadFromFile("Assets/Main menu & Loading/Loading Screen/Images/loadBarCap.png"))
        cout << "!!! Error: Could not find loadBarCap.png !!!" << endl;
    loadBar_empty_sprite.setTexture(loadBar_empty);
    loadBar_empty_sprite.setOrigin({loadBar_empty_sprite.getLocalBounds().size.x / 2.f, loadBar_empty_sprite.getLocalBounds().size.y / 2.f});
    loadBar_empty_sprite.setPosition({WindowWidth / 2.f, WindowHeight - 85.f});
    ScaleX = width / loadBar_empty.getSize().x;
    ScaleY = height / loadBar_empty.getSize().y;
    loadBar_empty_sprite.setScale({ScaleX, ScaleY});
    loadBar_empty.setSmooth(true);

    if (!loadBar.loadFromFile("Assets/Main menu & Loading/Loading Screen/Images/loadBar.png"))
        cout << "!!! Error: Could not find loadBar.png !!!" << endl;
    loadBar_sprite.setTexture(loadBar);
    loadBar_sprite.setOrigin({loadBar_sprite.getLocalBounds().size.x / 2.f, loadBar_sprite.getLocalBounds().size.y / 2.f});
    loadBar_sprite.setPosition({WindowWidth / 2.f, WindowHeight - 85.f});
    loadBar_sprite.setScale({width / loadBar.getSize().x, height / loadBar.getSize().y});
    loadBar.setSmooth(true);

    if (!bubbleSmallTex.loadFromFile("Assets/Main menu & Loading/Loading Screen/Images/fx_bubble_d0.png"))
        cout << "!!! Error: Could not find fx_bubble_d0.png !!!" << endl;
    bubbleSmallTex.setSmooth(true);
    bubbleSmallSprite.setTexture(bubbleSmallTex);

    if (!spark.loadFromFile("Assets/Main menu & Loading/Loading Screen/Images/sparkleb0.png"))
        cout << "!!! Error: Could not find sparkleb0.png !!!" << endl;
    spark.setSmooth(true);
    sparkSprite.setTexture(spark);
}

// Returns true when loading screen is fully done and we should move on
bool Draw_LoadingScreen(float totalTime)
{
    // Doc1: window.clear(Color::Black) — commented out, background sprite has priority
    // window.clear(Color::Black);
    swayShader.setUniform("time", totalTime);
    Background_Loading.setSmooth(true);
    window.draw(Background_Loading_sprite, &swayShader);
    window.draw(Game_Icon_sprite);
    window.draw(POPCAP_Logo_sprite);
    window.draw(LogoWood_sprite);

    float loadingStartTime = 1.0f;
    float progress = (totalTime - loadingStartTime) / 5.0f;
    if (progress < 0)
        progress = 0;
    if (progress > 1.0f)
        progress = 1.0f;

    Vector2u textureSize = loadBar.getSize();
    int currentWidth = (int)(textureSize.x * progress);
    loadBar_sprite.setTextureRect(IntRect({0, 0}, {currentWidth, (int)textureSize.y}));

    window.draw(loadBar_empty_sprite);
    window.draw(loadBar_sprite);

    startX_loading = WindowWidth / 2.0f - (3.5f * spacing) + 20.f;
    Sprite *letters[] = {&s_L, &s_O, &s_A, &s_D, &s_I, &s_N, &s_G};

    for (int i = 0; i < 7; i++)
    {
        float startDelay = 1.5f;
        float letterDelay = i * 0.4f;
        float letterTime = totalTime - startDelay - letterDelay;

        float popScale = 0.0f;
        if (letterTime > 0)
        {
            popScale = (letterTime < 0.4f) ? sin(letterTime * (3.14f / 0.4f)) * 1.2f : 1.0f;
            if (popScale < 1.0f && letterTime > 0.2f)
                popScale = 1.0f;
        }

        float curX = startX_loading + (i * spacing);
        float curY = (WindowHeight / 2.0f - 40.f) + sin(totalTime * 4.0f + i * 0.8f) * 12.0f;

        bubbleSprite.setScale({0.7f * popScale, 0.7f * popScale});
        letters[i]->setScale({0.92f * popScale, 0.92f * popScale});
        bubbleSprite.setPosition({curX, curY});
        letters[i]->setPosition({curX, curY});

        if (progress < 1.0f && popScale > 0)
        {
            window.draw(bubbleSprite);
            window.draw(*letters[i]);
        }
    }

    if (progress >= 1.0f && !exploded)
    {
        exploded = true;
        for (int i = 0; i < 35; i++)
        {
            int idx = i / 5;
            float pX = startX_loading + (idx * spacing);
            float pY = (WindowHeight / 2.0f - 40.f);

            sparks[i].active = true;
            sparks[i].x = pX;
            sparks[i].y = pY;
            sparks[i].vx = (float)(rand() % 60 - 20) / 15.0f;
            sparks[i].vy = -((float)(rand() % 30 + 10)) / 6.0f;
            sparks[i].alpha = 255.0f;

            letterBubbles[i].active = true;
            letterBubbles[i].x = pX;
            letterBubbles[i].y = pY;
            letterBubbles[i].vx = (float)(rand() % 60 - 20) / 18.0f;
            letterBubbles[i].vy = -((float)(rand() % 15 + 5)) / 5.0f;
            letterBubbles[i].alpha = 200.0f;
        }
    }

    if (exploded && !particlesFinished)
    {
        bool anyActive = false;

        for (int i = 0; i < 35; i++)
        {
            if (sparks[i].active)
            {
                anyActive = true;
                sparks[i].x += sparks[i].vx;
                sparks[i].vy += 0.2f;
                sparks[i].y += sparks[i].vy;
                sparks[i].alpha -= 6.f;
                if (sparks[i].alpha <= 0)
                {
                    sparks[i].alpha = 0;
                    sparks[i].active = false;
                }
                else
                {
                    sparkSprite.setPosition({sparks[i].x, sparks[i].y});
                    sparkSprite.setColor(Color(255, 255, 255, (uint8_t)sparks[i].alpha));
                    sparkSprite.setScale({0.55f, 0.55f});
                    window.draw(sparkSprite);
                }
            }

            if (letterBubbles[i].active)
            {
                anyActive = true;
                letterBubbles[i].x += letterBubbles[i].vx;
                letterBubbles[i].y += letterBubbles[i].vy;
                letterBubbles[i].alpha -= 7.f;
                if (letterBubbles[i].alpha <= 0)
                {
                    letterBubbles[i].alpha = 0;
                    letterBubbles[i].active = false;
                }
                else
                {
                    bubbleSmallSprite.setPosition({letterBubbles[i].x, letterBubbles[i].y});
                    bubbleSmallSprite.setColor(Color(255, 255, 255, (uint8_t)letterBubbles[i].alpha));
                    window.draw(bubbleSmallSprite);
                }
            }
        }
        if (!anyActive)
            particlesFinished = true;
    }

    timerLeft += deltaTime;
    timerRight += deltaTime;
    bubbleTimer += deltaTime;

    auto spawnBubble = [&](int ventIdx)
    {
        for (int i = 0; i < 60; i++)
        {
            if (!ventBubbles[i].active)
            {
                ventBubbles[i].active = true;
                ventBubbles[i].isVent = true;
                ventBubbles[i].alpha = 255.0f;
                ventBubbles[i].x = ventPositions[ventIdx].x + (rand() % 12 - 6);
                ventBubbles[i].y = ventPositions[ventIdx].y;
                ventBubbles[i].vx = (float)(rand() % 10 - 5) / 100.0f;
                ventBubbles[i].vy = -((float)(rand() % 20 + 20)) / 20.0f;
                return true;
            }
        }
        return false;
    };

    if (timerLeft >= 0.8f)
    {
        if (spawnBubble(0))
            timerLeft = 0.0f;
    }
    if (timerRight >= 0.8f)
    {
        if (spawnBubble(1))
            timerRight = 0.0f;
    }

    if (bubbleTimer >= 1.5f)
    {
        for (int i = 0; i < 60; i++)
        {
            if (!ventBubbles[i].active)
            {
                ventBubbles[i].active = true;
                ventBubbles[i].isVent = false;
                ventBubbles[i].alpha = 255.0f;
                ventBubbles[i].x = WindowWidth + 10.f;
                ventBubbles[i].y = (float)(rand() % (int)WindowHeight);
                ventBubbles[i].vx = -((float)(rand() % 30 + 15) / 10.f);
                ventBubbles[i].vy = 0.0f;
                bubbleTimer = 0.0f;
                break;
            }
        }
    }

    for (int i = 0; i < 60; i++)
    {
        if (ventBubbles[i].active)
        {
            ventBubbles[i].x += ventBubbles[i].vx;
            ventBubbles[i].y += ventBubbles[i].vy;
            if (ventBubbles[i].isVent)
            {
                ventBubbles[i].x += sin(totalTime * 4.0f + i) * 0.25f;
                if (ventBubbles[i].y < 200.f)
                    ventBubbles[i].alpha -= 8.0f;
                if (ventBubbles[i].alpha <= 0 || ventBubbles[i].y < 10.f)
                    ventBubbles[i].active = false;
            }
            else
            {
                if (ventBubbles[i].x < -50.f)
                    ventBubbles[i].active = false;
            }
            if (ventBubbles[i].active)
            {
                bubbleSmallSprite.setPosition({ventBubbles[i].x, ventBubbles[i].y});
                bubbleSmallSprite.setColor(Color(255, 255, 255, (uint8_t)ventBubbles[i].alpha));
                float s = ventBubbles[i].isVent ? (0.8f + (i % 3) * 0.2f) : 0.65f;
                bubbleSmallSprite.setScale({s, s});
                window.draw(bubbleSmallSprite);
            }
        }
    }

    if (particlesFinished)
    {
        if (!transitionStarted)
        {
            transitionStarted = true;
            endTimer = totalTime;
        }
        if (totalTime - endTimer >= 0.00001f)
        {
            window.clear(Color::Black);
            window.display();
            return true; // Signal: loading screen is done
        }
    }

    window.display();
    return false;
}

// ============================================================
// ==================== SWITCH USER (Doc3) ====================
// ============================================================

// Doc3 had its own RenderWindow window(1920x1080) — removed, using shared window above
// Doc3 had its own main() — removed, logic moved into SwitchUser() below

Texture FullTex;
Sprite Full(FullTex);
Texture TitleTex;
Sprite Title(TitleTex);
Texture NewButtonTex;
Sprite NewButton(NewButtonTex);
Texture NewButtonHLTex;
Sprite NewButtonHL(NewButtonHLTex);
Sprite SelectButton(NewButtonTex);
Sprite DeleteButton(NewButtonTex);
Texture BlinkTex;
Sprite Blink(BlinkTex);
Texture EnterYourNameBgTex;
Sprite EnterYourNamebg(EnterYourNameBgTex);
Font btnFont;
Text NewText(btnFont, "", 40);
Text SelectText(btnFont, "", 40);
Text DeleteText(btnFont, "", 40);
Font Inputfont;
Text DisplayText(Inputfont, "", 8);
bool NameEntry = 0;
string InputString = "";
bool drawtheCursor = 0;
Clock CursorTimer;
Text DoneAddingUserText(btnFont, "", 40);
Texture DoneAddingUserTex;
Sprite DoneAddingUser(DoneAddingUserTex);
pair<string, int> users[7];
int NumberOfUsers = 0;
const int MaxNumberOfUsers = 7;
int SelectedUser = -1;
Text *UserTexts[8];
RectangleShape SelectUserHL;
string CurUser = "";
bool isUserSelected = 0;
Texture CancelAddingUserTex;
Sprite CancelAddingUser(CancelAddingUserTex);
Text CancelAddingUserText(btnFont, "", 40);
Texture CancelAddingUserHLTex;
bool isCancelAddingUser = 0;
Texture DelteUserBgTex;
Sprite DeleteUserBg(DelteUserBgTex);
bool isConfirmUserDelete = 0;
Sprite YesButton(NewButtonTex);
Sprite NoButton(NewButtonTex);
Text YesButtonText(btnFont, "", 40);
Text NoButtonText(btnFont, "", 40);
bool JustOpenDeleteConfirm = 0;
Text DeletethisUser(btnFont, "", 40);
Texture ListisFullTex;
Sprite ListisFull(ListisFullTex);
bool isListFull = 0;
Text OKText(btnFont, "", 40);
bool DupplicateName = 0;
Texture FullOKButtonTex;
Sprite FullOKButton(FullOKButtonTex);
Texture DupplicateOKButtonTex;
Sprite DupplicateOKButton(DupplicateOKButtonTex);
Texture DupplicateBgTex;
Sprite DupplicateBg(DupplicateBgTex);
Text DupplicatedUserText(btnFont, "", 40);
Texture DupplicatedUserTex;
bool CamefromDupplicate = 0;

// Forward declarations for SwitchUser helpers
void SU_Start();
void SU_Update();
void SwitchUser_Display();
void SU_EnterYourName();
void SU_RefreshUsersList();
void SU_DeleteUser();
void SU_FullList();
void SU_DupplicateUser();
void SU_CreateSprite(Sprite &sprite, Texture &texture, const string &filePath,
                     float xPosition, float yPositon, float scaleX, float scaleY);
void SU_SetupText(Text &text, const string &str, Sprite &button);
void SU_HoverButton(Sprite &button, const Texture &normalTex,
                    const Texture &highlightTex, Vector2f mousePos);

void SwitchUser()
{
    SU_Start();
    // SwitchUser loop — runs until user selects a profile or closes
    while (window.isOpen())
    {
        while (auto x = window.pollEvent())
        {
            if (x->is<Event::Closed>())
                window.close();
            // Escape from SwitchUser back to Main Menu (no close)
            if (auto keyEvent = x->getIf<Event::KeyPressed>())
                if (keyEvent->code == Keyboard::Key::Escape)
                    return; // Go back to main menu
        }
        SU_Update();
        SwitchUser_Display();
        if (isUserSelected)
            return; // User picked a profile — return to main menu
    }
}

void SU_CreateSprite(Sprite &sprite, Texture &texture, const string &filePath,
                     float xPosition, float yPositon, float scaleX, float scaleY)
{
    if (!texture.loadFromFile(filePath))
        cout << "Failed to load: " << filePath << "\n";
    sprite = Sprite(texture);
    sprite.setOrigin({texture.getSize().x / 2.f, texture.getSize().y / 2.f});
    sprite.setPosition({xPosition, yPositon});
    sprite.setScale({scaleX, scaleY});
}

void SU_SetupText(Text &text, const string &str, Sprite &button)
{
    text.setFont(btnFont);
    text.setString(str);
    text.setCharacterSize(40);
    text.setFillColor(Color(210, 225, 90));
    text.setOutlineColor(Color(30, 60, 10));
    text.setOutlineThickness(2.f);
    FloatRect bounds = text.getLocalBounds();
    text.setOrigin({bounds.size.x / 2.f, bounds.size.y / 2.f});
    Vector2f center = {button.getPosition().x, button.getPosition().y - 10};
    text.setPosition(center);
}

void SU_Start()
{
    for (int i = 0; i < 8; i++)
        UserTexts[i] = nullptr;

    // Reset SwitchUser state on each entry
    isUserSelected = 0;
    NameEntry = 0;
    isConfirmUserDelete = 0;
    isListFull = 0;
    DupplicateName = 0;
    SelectedUser = -1;
    isCancelAddingUser = 0;
    InputString = "";
    NumberOfUsers = 0;

    float X = WindowWidth / 2.f, Y = WindowHeight / 2.f;
    SU_CreateSprite(Full, FullTex, "Assets/Switch User/FullPhoto.png", X, Y, 0.25, 0.25);

    X = WindowWidth * 0.37f;
    Y = WindowHeight * 0.68f;
    SU_CreateSprite(NewButton, NewButtonTex, "Assets/Switch User/shell_tinybtn124_normal.png", X, Y, 1.5, 1.5);
    if (!NewButtonHLTex.loadFromFile("Assets/Switch User/shell_tinybtn124_high.jpg"))
        cout << "Failed to load: ButtonHL\n";

    X = WindowWidth * 0.5f;
    Y = WindowHeight * 0.68f;
    SU_CreateSprite(SelectButton, NewButtonTex, "Assets/Switch User/shell_tinybtn124_normal.png", X, Y, 1.5, 1.5);

    X = WindowWidth * 0.62f;
    Y = WindowHeight * 0.68f;
    SU_CreateSprite(DeleteButton, NewButtonTex, "Assets/Switch User/shell_tinybtn124_normal.png", X, Y, 1.5, 1.5);

    if (!btnFont.openFromFile("Assets/Fonts/BarmenoBold.otf"))
        cout << "Can't load font\n";
    SU_SetupText(NewText, "New", NewButton);
    SU_SetupText(SelectText, "Select", SelectButton);
    SU_SetupText(DeleteText, "Delete", DeleteButton);

    X = WindowWidth * 0.5f;
    Y = WindowHeight * 0.25f;
    SU_CreateSprite(Title, TitleTex, "Assets/Switch User/shell_listboxbg_chooseuser.jpg", X, Y, 1.5, 1.5);

    ifstream loadusers("Assets/Switch User/Users_List.txt");
    string name;
    int id;
    while (loadusers >> name >> id && NumberOfUsers < MaxNumberOfUsers)
    {
        users[NumberOfUsers].first = name;
        users[NumberOfUsers].second = id;
        NumberOfUsers++;
    }
    loadusers.close();

    SU_RefreshUsersList();

    SelectUserHL.setSize({WindowWidth * 0.35f, 45.f});
    SelectUserHL.setFillColor(Color(180, 220, 255, 150));
    SelectUserHL.setOrigin({SelectUserHL.getSize().x / 2.f, 0.f});
}

void SU_Update()
{
    Vector2f mousePos = static_cast<Vector2f>(Mouse::getPosition(window));

    while (auto x = window.pollEvent())
    {
        if (x->is<Event::Closed>())
            window.close();

        if (auto mouseEvent = x->getIf<Event::MouseButtonReleased>())
            if (mouseEvent->button == Mouse::Button::Left)
            {
                if (NewButton.getGlobalBounds().contains(mousePos) && !isConfirmUserDelete && !isListFull)
                    SU_EnterYourName();

                if (NameEntry && DoneAddingUser.getGlobalBounds().contains(mousePos))
                {
                    if (NumberOfUsers < MaxNumberOfUsers)
                    {
                        NameEntry = 0;
                        isCancelAddingUser = 0;
                        for (auto &user : users)
                        {
                            if (user.first == InputString)
                            {
                                DupplicateName = 1;
                                break;
                            }
                        }
                        if (DupplicateName)
                        {
                            SU_DupplicateUser();
                            NameEntry = 0;
                            if (!CamefromDupplicate)
                                InputString = "";
                        }
                        if (!DupplicateName)
                        {
                            ofstream adduser("Assets/Switch User/Users_List.txt");
                            users[NumberOfUsers].first = InputString;
                            users[NumberOfUsers].second = NumberOfUsers + 1;
                            NumberOfUsers++;
                            InputString = "";
                            for (int i = 0; i < NumberOfUsers; i++)
                                adduser << users[i].first << '\t' << users[i].second << '\n';
                            adduser.close();
                            SU_RefreshUsersList();
                            CurUser = users[NumberOfUsers - 1].first;
                        }
                    }
                    else if (NumberOfUsers >= MaxNumberOfUsers)
                    {
                        NameEntry = 0;
                        SU_FullList();
                        if (isListFull)
                            if (mouseEvent->button == Mouse::Button::Left)
                                if (FullOKButton.getGlobalBounds().contains(mousePos))
                                    isListFull = 0;
                    }
                }
                else if (NameEntry && CancelAddingUser.getGlobalBounds().contains(mousePos))
                {
                    InputString = "";
                    NameEntry = 0;
                    isCancelAddingUser = 1;
                }

                for (int i = 0; i < NumberOfUsers; i++)
                {
                    if (UserTexts[i] != nullptr && UserTexts[i]->getGlobalBounds().contains(mousePos))
                    {
                        SelectedUser = i;
                        float X = WindowWidth * 0.5f - 5, Y = UserTexts[i]->getPosition().y - 5.f;
                        SelectUserHL.setPosition({X, Y});
                    }
                }

                if (SelectButton.getGlobalBounds().contains(mousePos) && !NameEntry && !isCancelAddingUser && !isListFull && !DupplicateName)
                {
                    CurUser = users[SelectedUser].first;
                    isUserSelected = 1;
                }

                if (SelectedUser != -1 && !NameEntry && !isConfirmUserDelete && DeleteButton.getGlobalBounds().contains(mousePos))
                    SU_DeleteUser();

                if (isConfirmUserDelete)
                {
                    if (!JustOpenDeleteConfirm && NoButton.getGlobalBounds().contains(mousePos))
                        isConfirmUserDelete = 0;
                    if (YesButton.getGlobalBounds().contains(mousePos))
                    {
                        for (int i = SelectedUser; i < NumberOfUsers - 1; i++)
                        {
                            users[i].first = users[i + 1].first;
                            users[i].second = i + 1;
                        }
                        users[NumberOfUsers - 1].first = "";
                        users[NumberOfUsers - 1].second = 0;
                        NumberOfUsers--;
                        ofstream rewrite("Assets/Switch User/Users_List.txt");
                        for (int i = 0; i < NumberOfUsers; i++)
                            rewrite << users[i].first << '\t' << users[i].second << '\n';
                        rewrite.close();
                        isConfirmUserDelete = 0;
                        SelectedUser = -1;
                        SU_RefreshUsersList();
                    }
                    JustOpenDeleteConfirm = 0;
                }

                if (FullOKButton.getGlobalBounds().contains(mousePos))
                    if (isListFull && !NameEntry && !isConfirmUserDelete && !DupplicateName)
                        isListFull = 0;

                if (DupplicateOKButton.getGlobalBounds().contains(mousePos))
                    if (DupplicateName && !isListFull && !isConfirmUserDelete && !NameEntry && !isUserSelected)
                    {
                        DupplicateName = 0;
                        CamefromDupplicate = 1;
                        NameEntry = 1;
                    }
            }

        if (const auto *typed = x->getIf<Event::TextEntered>())
        {
            char32_t a = typed->unicode;
            if (a == 8 && !InputString.empty())
                InputString.pop_back();
            else if (a >= 32 && InputString.size() < 8)
                InputString += (char)a;
        }
    }

    if (NameEntry)
    {
        if (CursorTimer.getElapsedTime() >= seconds(0.5f))
        {
            drawtheCursor = !drawtheCursor;
            CursorTimer.restart();
        }
        FloatRect textBounds = DisplayText.getLocalBounds();
        float X = DisplayText.getPosition().x + textBounds.size.x + 5;
        float Y = DisplayText.getPosition().y + 25;
        Blink.setPosition({X, Y});
        DisplayText.setString(InputString);
    }

    SU_HoverButton(NewButton, NewButtonTex, NewButtonHLTex, mousePos);
    SU_HoverButton(SelectButton, NewButtonTex, NewButtonHLTex, mousePos);
    SU_HoverButton(DeleteButton, NewButtonTex, NewButtonHLTex, mousePos);
    if (NameEntry && !isConfirmUserDelete)
    {
        SU_HoverButton(DoneAddingUser, NewButtonTex, NewButtonHLTex, mousePos);
        SU_HoverButton(CancelAddingUser, CancelAddingUserTex, CancelAddingUserHLTex, mousePos);
    }
    if (isConfirmUserDelete && !NameEntry)
    {
        SU_HoverButton(YesButton, NewButtonTex, NewButtonHLTex, mousePos);
        SU_HoverButton(NoButton, NewButtonTex, NewButtonHLTex, mousePos);
    }
    if (!NameEntry && !isConfirmUserDelete && isListFull && !DupplicateName)
        SU_HoverButton(FullOKButton, NewButtonTex, NewButtonHLTex, mousePos);
    if (!NameEntry && !isConfirmUserDelete && !isListFull && DupplicateName)
        SU_HoverButton(DupplicateOKButton, NewButtonTex, NewButtonHLTex, mousePos);
}

void SU_HoverButton(Sprite &button, const Texture &normalTex,
                    const Texture &highlightTex, Vector2f mousePos)
{
    if (button.getGlobalBounds().contains(mousePos))
        button.setTexture(highlightTex);
    else
        button.setTexture(normalTex);
}

void SU_EnterYourName()
{
    if (NameEntry)
        return;
    isUserSelected = 0;
    NameEntry = 1;
    unsigned int X = WindowWidth * 0.5f, Y = WindowHeight * 0.5f;
    SU_CreateSprite(EnterYourNamebg, EnterYourNameBgTex, "Assets/Switch User/shell_popupfg_entername.png", X, Y, 0.25, 0.25);

    if (!Inputfont.openFromFile("Assets/Fonts/trebuc.ttf"))
        cout << "Cant Open Font!";
    DisplayText.setFont(Inputfont);
    DisplayText.setCharacterSize(40);
    DisplayText.setFillColor(Color::White);
    X = WindowWidth * 0.38f;
    Y = WindowHeight * 0.42f;
    DisplayText.setPosition({(float)X, (float)Y});

    X = WindowWidth * 0.375f;
    Y = WindowHeight * 0.448f;
    SU_CreateSprite(Blink, BlinkTex, "Assets/Switch User/shell_editboxcursor.jpg", X, Y, 1.5, 1.5);

    X = WindowWidth * 0.5f;
    Y = WindowHeight * 0.58f;
    SU_CreateSprite(DoneAddingUser, DoneAddingUserTex, "Assets/Switch User/shell_tinybtn124_normal.png", X, Y, 1.5, 1.5);
    SU_SetupText(DoneAddingUserText, "Done", DoneAddingUser);

    if (!CancelAddingUserHLTex.loadFromFile("Assets/Switch User/shell_tinybtn124_high.jpg"))
        cout << "Can't load Texture!\n";
    X = WindowWidth * 0.5f;
    Y = WindowHeight * 0.68f;
    SU_CreateSprite(CancelAddingUser, CancelAddingUserTex, "Assets/Switch User/shell_tinybtn124_normal.jpg", X, Y, 1.5, 1.5);
    SU_SetupText(CancelAddingUserText, "Cancel", CancelAddingUser);
}

void SU_RefreshUsersList()
{
    for (int i = 0; i < NumberOfUsers; i++)
    {
        delete UserTexts[i];
        UserTexts[i] = nullptr;
        if (i < NumberOfUsers)
        {
            UserTexts[i] = new Text(btnFont, users[i].first, 35);
            UserTexts[i]->setFillColor(Color::White);
            float X = WindowWidth * 0.5f;
            float Y = WindowHeight * 0.3f + (i * 50.f);
            UserTexts[i]->setPosition({X, Y});
            FloatRect bounds = UserTexts[i]->getLocalBounds();
            UserTexts[i]->setOrigin({bounds.size.x / 2.f, 0.f});
        }
        else
            UserTexts[i] = nullptr;
    }
}

void SU_DeleteUser()
{
    JustOpenDeleteConfirm = 1;
    isConfirmUserDelete = 1;
    unsigned int X = WindowWidth * 0.5f, Y = WindowHeight * 0.5f;
    SU_CreateSprite(DeleteUserBg, DelteUserBgTex, "Assets/Switch User/shell_popupfg_deleteuser.jpg", X, Y, 0.25, 0.25);
    X = WindowWidth * 0.4f;
    Y = WindowHeight * 0.65f;
    SU_CreateSprite(YesButton, NewButtonTex, "Assets/Switch User/shell_tinybtn124_normal.png", X, Y, 1.5, 1.5);
    SU_SetupText(YesButtonText, "Yes", YesButton);
    X = WindowWidth * 0.6f;
    Y = WindowHeight * 0.65f;
    SU_CreateSprite(NoButton, NewButtonTex, "Assets/Switch User/shell_tinybtn124_normal.png", X, Y, 1.5, 1.5);
    SU_SetupText(NoButtonText, "No!", NoButton);
    SU_SetupText(DeletethisUser, users[SelectedUser].first, DeleteUserBg);
    X = WindowWidth * 0.49f;
    Y = WindowHeight * 0.53f;
    DeletethisUser.setPosition({(float)X, (float)Y});
    DeletethisUser.setCharacterSize(60);
}

void SU_FullList()
{
    isListFull = 1;
    unsigned int X = WindowWidth * 0.5f, Y = WindowHeight * 0.5f;
    SU_CreateSprite(ListisFull, ListisFullTex, "Assets/Switch User/ListIsFull.png", X, Y, 0.25, 0.25);
    X = WindowWidth * 0.5f;
    Y = WindowHeight * 0.64f;
    SU_CreateSprite(FullOKButton, FullOKButtonTex, "Assets/Switch User/shell_tinybtn124_normal.png", X, Y, 1.5, 1.5);
    SU_SetupText(OKText, "OK", FullOKButton);
    InputString = "";
}

void SU_DupplicateUser()
{
    DupplicateName = 1;
    unsigned int X = WindowWidth * 0.5f, Y = WindowHeight * 0.5f;
    SU_CreateSprite(DupplicateBg, DupplicateBgTex, "Assets/Switch User/DupplicateUser.png", X, Y, 0.25, 0.25);
    X = WindowWidth * 0.5f;
    Y = WindowHeight * 0.64f;
    SU_CreateSprite(DupplicateOKButton, DupplicateOKButtonTex, "Assets/Switch User/shell_tinybtn124_normal.png", X, Y, 1.5, 1.5);
    SU_SetupText(OKText, "OK", DupplicateOKButton);
    SU_SetupText(DupplicatedUserText, InputString, DupplicateBg);
    X = WindowWidth * 0.49f;
    Y = WindowHeight * 0.53f;
    DupplicatedUserText.setPosition({(float)X, (float)Y});
    DupplicatedUserText.setCharacterSize(60);
}

void SwitchUser_Display()
{
    window.draw(Full);
    window.draw(Title);
    window.draw(NewButton);
    window.draw(SelectButton);
    window.draw(DeleteButton);
    window.draw(NewText);
    window.draw(SelectText);
    window.draw(DeleteText);

    for (int i = 0; i < NumberOfUsers; i++)
        if (UserTexts[i] != nullptr)
            window.draw(*UserTexts[i]);

    if (SelectedUser >= 0)
        window.draw(SelectUserHL);

    if (isUserSelected)
        return;

    if (NameEntry && !isConfirmUserDelete && !isListFull && !DupplicateName)
    {
        window.draw(EnterYourNamebg);
        window.draw(DisplayText);
        if (drawtheCursor)
            window.draw(Blink);
        window.draw(DoneAddingUser);
        window.draw(DoneAddingUserText);
        window.draw(CancelAddingUser);
        window.draw(CancelAddingUserText);
    }
    if (isConfirmUserDelete && SelectedUser != -1 && !NameEntry && !isListFull && !DupplicateName)
    {
        window.draw(DeleteUserBg);
        window.draw(YesButton);
        window.draw(NoButton);
        window.draw(YesButtonText);
        window.draw(NoButtonText);
        window.draw(DeletethisUser);
    }
    if (isListFull && !isConfirmUserDelete && !NameEntry && !DupplicateName && !isUserSelected)
    {
        window.draw(ListisFull);
        window.draw(FullOKButton);
        window.draw(OKText);
    }
    if (DupplicateName && !isListFull && !isConfirmUserDelete && !NameEntry && !isUserSelected)
    {
        window.draw(DupplicateBg);
        window.draw(DupplicateOKButton);
        window.draw(OKText);
        window.draw(DupplicatedUserText);
    }
    window.display();
}

// ============================================================
// ==================== MAIN MENU ======================
// ============================================================

float getRandom(float min, float max)
{
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dist(min, max);
    return dist(gen);
}

Texture mainbackground("Assets/Main menu & Loading/Main menu/mm_mainmenubg.jpg");
Sprite background(mainbackground);

Texture logo("Assets/Main menu & Loading/Main menu/logo-removebg-preview.png");
Sprite logosp(logo);

Texture Greenfishswim("Assets/Fish/greenfish/greenfishs.png");
Texture Greenfisheat("Assets/Fish/greenfish/dd.png");
Texture Greenfishturn("Assets/Fish/greenfish/fishturning.png");
Sprite Greenfish(Greenfishswim);
float GFvelocityX_AXIS = -2;
float GFvelocityY_AXIS = -2;
int GFswim = 0;
int GFeat = 0;
int GFturn = 0;
float GFchangedir = -1;
int counter = 0;

Texture Barracudatex("Assets/Fish/Barracuda/Barracuda.png");
Sprite Barracuda(Barracudatex);
int BFcol = 0;
int BFrow = 0;
float BFvelocityX_AXIS = -2;
float BFvelocityY_AXIS = -2;
float BFchangedir = -1;

Texture startgametex("Assets/Main menu & Loading/Main menu/mm_startgame_normal-removebg-preview.png");
Texture startgamepressed("Assets/Main menu & Loading/Main menu/mm_startgame_high-removebg-preview.png");
Sprite startgamebutton(startgametex);

SoundBuffer buttonpressed("Assets/Music and Sounds/mouseover.ogg");
Sound buttonpressedsound(buttonpressed);

Texture timeattacktex("Assets/Main menu & Loading/Main menu/mm_timeattack_normal-removebg-preview.png");
Texture timeattackpressed("Assets/Main menu & Loading/Main menu/mm_timeattack_high-removebg-preview.png");
Sprite timeattackbutton(timeattacktex);

Texture highscoretex("Assets/Main menu & Loading/Main menu/mm_highscores_normal-removebg-preview.png");
Texture highscorepressed("Assets/Main menu & Loading/Main menu/mm_highscores_high-removebg-preview.png");
Sprite highscorebutton(highscoretex);

Texture quittex("Assets/Main menu & Loading/Main menu/mm_quit_normal-removebg-preview.png");
Texture quitpressed("Assets/Main menu & Loading/Main menu/mm_quit_high-removebg-preview.png");
Sprite quitbutton(quittex);

Texture optionstex("Assets/Main menu & Loading/Main menu/mm_options_normal-removebg-preview.png");
Texture optionspressed("Assets/Main menu & Loading/Main menu/mm_options_high-removebg-preview.png");
Sprite optionsbutton(optionstex);

Texture switchusertex("Assets/Main menu & Loading/Main menu/mm_switchuser_normal-removebg-preview.png");
Texture switchuserpressed("Assets/Main menu & Loading/Main menu/mm_switchuser_high-removebg-preview.png");
Sprite switchuserbutton(switchusertex);

Texture creditstex("Assets/Main menu & Loading/Main menu/mm_credits_normal-removebg-preview.png");
Texture creditspressed("Assets/Main menu & Loading/Main menu/mm_credits_high-removebg-preview.png");
Sprite creditsbutton(creditstex);

Texture minowfishtex("Assets/Fish/herfish/herFish.png");
Sprite Minowfish(minowfishtex);
int MFcol = 0;
int MFrow = 0;
float MFchangedir = -1;
float MFvelocityX_AXIS = -2;
float MFvelocityY_AXIS = -2;
bool MFvisable = 1;
RectangleShape MFcollieder(Vector2f{57, 25});

float smallfishsvelocityYaxis = getRandom(-2.f, 2.f);
CircleShape b({20});

struct AnimatedObject
{
    float changedir = 1;
    float velocityX_AXIS = -2;
    float velocityY_AXIS = -2;

    AnimatedObject(const Texture &texture, int frameWidth, int frameHeight)
        : sprite(texture, IntRect({0, 0}, {frameWidth, frameHeight}))
    {
    }

    Sprite sprite;
    int currentFrame = 0;
    float frameTime = 0.1f;
    float timer = 0.0f;
    Vector2f velocity;
    RectangleShape shape;

    void update(int frameWidth, int frameHeight)
    {
        sprite.move({velocityX_AXIS, 0});
        timer = 0.0f;
        sprite.setTextureRect(IntRect({currentFrame * frameWidth, 0}, {frameWidth, frameHeight}));
        currentFrame++;
        if (currentFrame == 15)
            currentFrame = 0;
    }
};

vector<AnimatedObject> smallfishs;

RectangleShape Barracudacollieder(Vector2f{271, 121});
RectangleShape QueenTriggercollieder(Vector2f{260, 160});
int cu = 0;

float totaltime_mm = 0.0f;
Vector2i mouseLocalPos;
Vector2f mouseWorldPos;
CircleShape c({10});

Texture QueenTrigerTEX("Assets/Fish/QueenTrigger/QueenTrigger.png");
Sprite QueenTrigger(QueenTrigerTEX);
int QTcol = 0;
int QTrow = 0;
float QTchangedir = -1;
float QTvelocityX_AXIS = -2;
float QTvelocityY_AXIS = -2;
RectangleShape n;
int num = 0;

void MM_Start();
void MM_Update();
void MM_Draw();
void MM_GreenfishAnimation();
void MM_MinowFishanimation();
void MM_BarracudaFishanimation();
void MM_ChangingButtonShape();
void MM_QueenTriggerFish();

void MM_Start()
{
    srand(time(0));

    for (int i = 0; i < 3; i++)
    {
        AnimatedObject obj{minowfishtex, 286, 126};
        obj.velocityX_AXIS = -2;
        obj.changedir = 1;
        obj.shape.setFillColor(Color::Green);
        obj.shape.setSize(Vector2f{57, 25});
        obj.shape.setOrigin(obj.shape.getLocalBounds().size / 2.f);
        obj.sprite.setTexture(minowfishtex);
        obj.sprite.setOrigin({285 / 2, 126 / 2});
        obj.sprite.setPosition(Vector2f({window.getSize().x / 2.0f + 10000, window.getSize().y / 2.0f + (i * 50)}));
        if (i == 0)
            obj.sprite.setPosition({1100, 360});
        if (i == 2)
            obj.sprite.setPosition({900, 360});
        obj.sprite.setScale({float(obj.changedir * 0.2), 0.2});
        obj.velocity = Vector2f({obj.velocityX_AXIS, 0});
        smallfishs.push_back(obj);
    }

    window.setFramerateLimit(30);

    logo.setSmooth(true);
    mainbackground.setSmooth(true);
    timeattackpressed.setSmooth(true);
    timeattacktex.setSmooth(true);
    optionstex.setSmooth(true);
    optionspressed.setSmooth(true);
    quittex.setSmooth(true);
    quitpressed.setSmooth(true);
    highscorepressed.setSmooth(true);
    highscoretex.setSmooth(true);
    creditstex.setSmooth(true);
    creditspressed.setSmooth(true);
    switchusertex.setSmooth(true);
    switchuserpressed.setSmooth(true);
    Greenfishturn.setSmooth(true);
    Greenfishswim.setSmooth(true);
    Barracudatex.setRepeated(true);
    QueenTrigerTEX.setSmooth(true);
    startgametex.setSmooth(true);
    startgamepressed.setSmooth(true);

    background.setTexture(mainbackground);
    background.setOrigin(background.getLocalBounds().size / 2.0f);
    background.setPosition(Vector2f({window.getSize().x / 2.0f, window.getSize().y / 2.0f}));
    background.setScale(Vector2f({1.6f, 1.2f}));

    logosp.setOrigin(logosp.getLocalBounds().size / 2.0f);
    logosp.setPosition(Vector2f({window.getSize().x / 2.0f, window.getSize().y / 2.0f - 240}));
    logosp.setScale(Vector2f({1.2f, 1.1f}));

    Barracudacollieder.setFillColor(Color::Green);
    Barracudacollieder.setOrigin(Barracudacollieder.getLocalBounds().size / 2.0f);

    MFcollieder.setFillColor(Color::Green);
    MFcollieder.setOrigin(MFcollieder.getLocalBounds().size / 2.0f);

    Greenfish.setPosition(Vector2f({window.getSize().x / 2.0f - 300, window.getSize().y / 2.0f}));
    Greenfish.setOrigin(Greenfish.getLocalBounds().size / 26.f);
    Greenfish.setScale(Vector2f({1.0f, 1.0f}));

    startgamebutton.setOrigin(startgamebutton.getLocalBounds().size / 2.0f);
    startgamebutton.setPosition(Vector2f({window.getSize().x / 2.0f, window.getSize().y / 2.0f - 80}));
    startgamebutton.setScale({1.5, 1.5});

    timeattackbutton.setOrigin(timeattackbutton.getLocalBounds().size / 2.0f);
    timeattackbutton.setPosition(Vector2f({window.getSize().x / 2.0f + 10, window.getSize().y / 2.0f + 50}));
    timeattackbutton.setScale({1.5, 1.5});

    highscorebutton.setOrigin(highscorebutton.getLocalBounds().size / 2.0f);
    highscorebutton.setPosition(Vector2f({window.getSize().x / 2.0f - 150, window.getSize().y / 2.0f + 140}));
    highscorebutton.setScale({1.5, 1.5});

    optionsbutton.setOrigin(optionsbutton.getLocalBounds().size / 2.0f);
    optionsbutton.setPosition(Vector2f({window.getSize().x / 2.0f + 120, window.getSize().y / 2.0f + 150}));
    optionsbutton.setScale({1.5, 1.5});

    quitbutton.setOrigin(quitbutton.getLocalBounds().size / 2.0f);
    quitbutton.setPosition(Vector2f({window.getSize().x / 2.0f, window.getSize().y / 2.0f + 230}));
    quitbutton.setScale({1.5, 1.5});

    switchuserbutton.setOrigin(switchuserbutton.getLocalBounds().size / 2.0f);
    switchuserbutton.setPosition(Vector2f({window.getSize().x / 2.0f + 460, window.getSize().y / 2.0f + 300}));
    switchuserbutton.setScale({1.5, 1.5});

    creditsbutton.setOrigin(creditsbutton.getLocalBounds().size / 2.0f);
    creditsbutton.setPosition(Vector2f({window.getSize().x / 2.0f - 450, window.getSize().y / 2.0f + 290}));
    creditsbutton.setScale({1.5, 1.5});

    Minowfish.setPosition(Vector2f({window.getSize().x / 2.0f - 300, window.getSize().y / 2.0f}));
    Minowfish.setOrigin({285 / 2, 126 / 2});
    Minowfish.setScale({0.2, 0.2});

    Barracuda.setPosition(Vector2f({window.getSize().x / 2.0f + 400, window.getSize().y / 2.0f}));
    Barracuda.setOrigin(Barracuda.getLocalBounds().size / 28.f);

    QueenTrigger.setOrigin({298 / 2, 216 / 2});
    QueenTriggercollieder.setFillColor(Color::Green);
    QueenTriggercollieder.setOrigin(QueenTriggercollieder.getLocalBounds().size / 2.0f);
    QueenTrigger.setPosition({1450, 360});

    b.setFillColor(Color::Red);
    b.setOrigin(b.getLocalBounds().size / 2.f);
    b.setPosition({1280, 720});
}

void MM_Update()
{
    float smallfishsvelocityYaxis = getRandom(-2.f, 2.f);

    Barracudacollieder.setPosition(Vector2f(Barracuda.getPosition().x, Barracuda.getPosition().y + 50));
    QueenTriggercollieder.setPosition(Vector2f(QueenTrigger.getPosition().x, QueenTrigger.getPosition().y));
    MFcollieder.setPosition(Vector2f(Minowfish.getPosition().x, Minowfish.getPosition().y));

    Vector2i mouseLocalPos = Mouse::getPosition(window);
    Vector2f mouseWorldPos = window.mapPixelToCoords(mouseLocalPos);

    for (auto &obj : smallfishs)
    {
        obj.shape.setPosition({obj.sprite.getPosition().x, obj.sprite.getPosition().y});
        obj.update(286, 126);
        // Doc2 used win.getPosition() for wall checks — updated to window
        if (obj.sprite.getPosition().x == window.getPosition().x - 580 ||
            obj.sprite.getPosition().x == window.getPosition().y + 580)
        {
            obj.velocityX_AXIS *= -1;
            obj.changedir *= -1;
            obj.velocity = Vector2f({obj.velocityX_AXIS, 0});
            obj.sprite.setScale({float(0.2 * obj.changedir), 0.2});
        }
    }

    MM_GreenfishAnimation();
    MM_MinowFishanimation();
    MM_ChangingButtonShape();
    MM_BarracudaFishanimation();
    MM_QueenTriggerFish();
}

void MM_Draw()
{
    window.draw(background);
    window.draw(Greenfish);
    window.draw(MFcollieder);           // Doc2: debug collider, commented out as-is
    window.draw(QueenTriggercollieder); // Doc2: debug collider, commented out as-is
    window.draw(QueenTrigger);
    window.draw(Minowfish);
    for (auto &obj : smallfishs)
        window.draw(obj.sprite);
    window.draw(n);
    window.draw(Barracudacollieder); // Doc2: debug collider, commented out as-is
    window.draw(Barracuda);
    window.draw(b);
    window.draw(startgamebutton);
    window.draw(timeattackbutton);
    window.draw(highscorebutton);
    window.draw(optionsbutton);
    window.draw(quitbutton);
    window.draw(switchuserbutton);
    window.draw(creditsbutton);
    window.draw(logosp);
    window.display();
}

void MM_GreenfishAnimation()
{
    Greenfish.move({GFvelocityX_AXIS, GFvelocityY_AXIS});
    if (Greenfish.getPosition().x == window.getSize().x / 2.0f - 580 ||
        Greenfish.getPosition().x == window.getSize().x / 2.0f + 580)
    {
        counter = 0;
        while (true)
        {
            Greenfish.setTexture(Greenfishturn);
            Greenfish.setTextureRect(IntRect({GFturn * 227, 0}, {227, 255}));
            GFturn++;
            if (GFturn == 13)
            {
                GFturn = 0;
                Greenfish.setScale({GFchangedir, 1});
                GFvelocityX_AXIS = GFvelocityX_AXIS * -1;
                break;
            }
        }
        GFchangedir *= -1;
    }
    else
    {
        Greenfish.setTexture(Greenfishswim);
        Greenfish.setTextureRect(IntRect({GFswim * 227, 0}, {227, 233}));
        GFswim = (GFswim + 1) % 13;
    }

    if (Greenfish.getGlobalBounds().findIntersection(Minowfish.getGlobalBounds()))
    {
        if (counter < 1)
            GFeat = 0;
        if (counter == 4)
            Minowfish.setPosition({3000, 3000});
        counter++;
        if (counter <= 5)
        {
            Greenfish.setTexture(Greenfisheat);
            Greenfish.setTextureRect(IntRect({GFeat * 227, 0}, {227, 233}));
            GFeat++;
        }
        else
        {
            Greenfish.setTexture(Greenfishswim);
            Greenfish.setTextureRect(IntRect({GFswim * 227, 0}, {227, 233}));
            GFswim = (GFswim + 1) % 13;
        }
    }
}

void MM_ChangingButtonShape()
{
    Vector2i mouseLocalPos = Mouse::getPosition(window);
    Vector2f mouseWorldPos = window.mapPixelToCoords(mouseLocalPos);

    if (startgamebutton.getGlobalBounds().contains(mouseWorldPos))
    {
        startgamebutton.setTexture(startgamepressed);
        buttonpressedsound.play();
    }
    else
    {
        startgamebutton.setTexture(startgametex);
    }
    if (timeattackbutton.getGlobalBounds().contains(mouseWorldPos))
        timeattackbutton.setTexture(timeattackpressed);
    else
        timeattackbutton.setTexture(timeattacktex);
    if (highscorebutton.getGlobalBounds().contains(mouseWorldPos))
        highscorebutton.setTexture(highscorepressed);
    else
        highscorebutton.setTexture(highscoretex);
    if (optionsbutton.getGlobalBounds().contains(mouseWorldPos))
        optionsbutton.setTexture(optionspressed);
    else
        optionsbutton.setTexture(optionstex);
    if (quitbutton.getGlobalBounds().contains(mouseWorldPos))
        quitbutton.setTexture(quitpressed);
    else
        quitbutton.setTexture(quittex);
    if (switchuserbutton.getGlobalBounds().contains(mouseWorldPos))
        switchuserbutton.setTexture(switchuserpressed);
    else
        switchuserbutton.setTexture(switchusertex);
    if (creditsbutton.getGlobalBounds().contains(mouseWorldPos))
        creditsbutton.setTexture(creditspressed);
    else
        creditsbutton.setTexture(creditstex);
}

void MM_MinowFishanimation()
{
    if (MFvisable)
    {
        Minowfish.move({MFvelocityX_AXIS, 0});
        if (Minowfish.getPosition().x == window.getSize().x / 2.0f - 580 ||
            Minowfish.getPosition().x == window.getSize().x / 2.0f + 580)
        {
            while (true)
            {
                Minowfish.setTextureRect(IntRect({MFcol * 286, 1 * 126}, {286, 126}));
                if (MFcol == 6)
                {
                    Minowfish.setScale({float(0.2 * MFchangedir), 0.2});
                    MFvelocityX_AXIS = MFvelocityX_AXIS * -1;
                    break;
                }
                MFcol++;
            }
            MFchangedir *= -1;
        }
        else
        {
            Minowfish.setTextureRect(IntRect({MFcol * 286, 0}, {286, 126}));
            MFcol = (MFcol + 1) % 15;
        }
    }
}

void MM_BarracudaFishanimation()
{
    Barracuda.move({BFvelocityX_AXIS, 0});
    BFrow = 0;
    if (Barracuda.getPosition().x == window.getSize().x / 2.0f - 580 ||
        Barracuda.getPosition().x == window.getSize().x / 2.0f + 580)
    {
        counter = 0;
        BFcol = 0;
        while (true)
        {
            Barracuda.setTextureRect(IntRect({BFcol * 256, 2 * 118}, {256, 118}));
            BFcol++;
            if (BFcol == 5)
            {
                BFcol = 0;
                Barracuda.setScale({BFchangedir, 1});
                BFvelocityX_AXIS = BFvelocityX_AXIS * -1;
                break;
            }
        }
        BFchangedir *= -1;
    }
    else if (Barracudacollieder.getGlobalBounds().findIntersection(MFcollieder.getGlobalBounds()))
    {
        if (counter < 1)
            BFcol = 0;
        if (counter == 4)
            Minowfish.setPosition({3000, 3000});
        counter++;
        if (counter <= 6)
        {
            Barracuda.setTextureRect(IntRect({BFcol * 256, 0}, {256, 118}));
            BFcol++;
        }
        else
        {
            Barracuda.setTextureRect(IntRect({BFcol * 256, 1 * 118}, {256, 118}));
            BFcol = (BFcol + 1) % 14;
        }
    }
    else
    {
        Barracuda.setTextureRect(IntRect({BFcol * 256, 1 * 118}, {256, 118}));
        BFcol = (BFcol + 1) % 14;
    }
    if (Barracuda.getPosition().y == 690 || Barracuda.getPosition().y == 0)
        BFvelocityY_AXIS *= -1;
}

void MM_QueenTriggerFish()
{
    QueenTrigger.move({QTvelocityX_AXIS, 0});
    QTrow = 0;
    if (QueenTrigger.getPosition().x == 0 || QueenTrigger.getPosition().x == 1460)
    {
        cu = 0;
        QTcol = 0;
        while (true)
        {
            QueenTrigger.setTextureRect(IntRect({QTcol * 298, 2 * 216}, {298, 216}));
            QTcol++;
            if (QTcol == 6)
            {
                QTcol = 0;
                QueenTrigger.setScale({QTchangedir, 1});
                QTvelocityX_AXIS = QTvelocityX_AXIS * -1;
                break;
            }
        }
        QTchangedir *= -1;
    }
    else if (!smallfishs.empty() &&
             (QueenTriggercollieder.getGlobalBounds().findIntersection(smallfishs[num].sprite.getGlobalBounds()) ||
              QueenTriggercollieder.getGlobalBounds().findIntersection(smallfishs[2].sprite.getGlobalBounds())))
    {
        if (cu < 1)
            QTcol = 0;
        if (cu == 3)
        {
            smallfishs[2].sprite.setPosition({3000, 3000});
            smallfishs[0].sprite.setPosition({3000, 3000});
        }
        cu++;
        if (cu <= 6)
        {
            QueenTrigger.setTextureRect(IntRect({QTcol * 298, 0}, {298, 216}));
            QTcol++;
        }
    }
    else
    {
        cu = 0;
        QueenTrigger.setTextureRect(IntRect({QTcol * 298, 1 * 216}, {298, 216}));
        QTcol = (QTcol + 1) % 14;
    }
}

// ============================================================
// ==================== MAIN (unified entry point) ============
// ============================================================

int main()
{
    cout << "SFML 3.0 and Standard Library are working!" << endl;

    // --- View setup ---
    view.setSize({WindowWidth, WindowHeight});
    view.setCenter({WindowWidth / 2.f, WindowHeight / 2.f});
    window.setView(view);

    // ── Loading Screen ──
    loading_screen_init();
    Clock clock;
    Clock totalClock;

    while (window.isOpen())
    {
        deltaTime = clock.restart().asSeconds();
        float totalTime = totalClock.getElapsedTime().asSeconds();

        while (const optional event = window.pollEvent())
        {
            if (event->is<Event::Closed>())
                window.close();
            else if (const auto *resizeEvent = event->getIf<Event::Resized>())
            {
                float windowRatio = (float)resizeEvent->size.x / (float)resizeEvent->size.y;
                float viewRatio = WindowWidth / WindowHeight;
                float sizeX = 1.f, sizeY = 1.f, posX = 0.f, posY = 0.f;
                if (windowRatio > viewRatio)
                {
                    sizeX = viewRatio / windowRatio;
                    posX = (1.f - sizeX) / 2.f;
                }
                else
                {
                    sizeY = windowRatio / viewRatio;
                    posY = (1.f - sizeY) / 2.f;
                }
                view.setViewport(sf::FloatRect({posX, posY}, {sizeX, sizeY}));
            }
        }

        window.setView(view);
        window.clear(Color::Black);

        bool done = Draw_LoadingScreen(totalTime);
        if (done)
            break; // Loading screen finished → move to main menu
    }

    // ── Main Menu ──
    MM_Start();
    window.setFramerateLimit(30);

    while (window.isOpen())
    {
        deltaTime = clock.restart().asSeconds();

        while (const optional event = window.pollEvent())
        {
            if (event->is<Event::Closed>())
                window.close();
            if (event->is<Event::KeyPressed>())
            {
                if (event->getIf<Event::KeyPressed>()->code == Keyboard::Key::Escape)
                    window.close();
            }

            // ── Switch User button click handler ──
            if (auto mouseEvent = event->getIf<Event::MouseButtonReleased>())
                if (mouseEvent->button == Mouse::Button::Left)
                {
                    Vector2i mPos = Mouse::getPosition(window);
                    Vector2f mPosF = window.mapPixelToCoords(mPos);
                    if (switchuserbutton.getGlobalBounds().contains(mPosF))
                    {
                        SwitchUser(); // Call SwitchUser, returns when user selects/cancels
                    }
                    if (quitbutton.getGlobalBounds().contains(mPosF))
                        window.close();
                }
        }

        background.setTexture(mainbackground);
        MM_Update();
        MM_Draw();
    }

    return 0;
}