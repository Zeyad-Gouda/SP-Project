#include <SFML/Graphics.hpp>
#include <SFML/Main.hpp>
#include <SFML/Network.hpp>
#include <SFML/System.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include <cctype>
#include <cmath>
#include <array>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <algorithm>
#include <string>
#include <random>
#include <optional>
#include <cstdint>
#include <fstream>
#include <iomanip>
#include <sstream>
using namespace std;
using namespace sf;

void loading_screen();
void StartLoadingScreen();
void UpdateLoadingScreen();
bool DrawLoadingScreen(float totalTime);
void MainMenu();
void StartMainMenu();
void UpdateMainMenu();
void DrawMainMenu();
void MainMenuFishAnimation();
void GreenfishAnimation();
void MinowFishanimation();
void BarracudaFishanimation();
void ChangingButtonShape();
void QueenTriggerFish();
void SwitchUser();
void ResetStats();
void StartSwitchUser();
void CreateButton(Sprite& sprite, Texture& texture, const string& filePath,
    float xPosition,
    float yPositon, float scaleX, float scaleY);
void SetupButtonText(Text& text, const string& str, Sprite& button);
void UpdateSwitchUser();
void HoverButton(Sprite& button, const Texture& normalTex,
    const Texture& highlightTex, Vector2f mousePos, Text& text);
void EnterYourName();
void RefreshUsersList();
void DeleteUser();
void FullList();
void DupplicateUser();
void DisplaySwitchUser();
void Select_level();
void StartSelectLevel();
void UpdateSelectLevel(float dt = 0.f);
void DrawSelectLevel();
void OptionsMenu();
void StartOptions();
void UpdateOptions();
void DrawOptions();
void UpdateMainMenuFish();
void DrawMainMenuBackground();
void PlayingSound(bool isMainMenu);
void QuitGame();
void StartQuit();
void UpdateQuit();
void DrawQuit();
void FadeOutToBlack();
void FadeInFromBlack();
string formatScore(int s);
void saveAllScores();
void loadAllScores();
void updateHighScoreTexts();
void resetScores();
void addNewHighScore(string name, int score, bool isStoryMode);
void Highscore();
void StartHighscore();
void UpdateHighscore();
void DrawHighscore();
void Credits();
void StartCredits();
bool UpdateCredits();
void DrawCredits();
void GameScreen(int level);
void StartGameScreen(int level);
void UpdateGameScreen();
void DrawGameScreen();

float getRandom(float min, float max)
{
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dist(min, max);
    return dist(gen);
}

float WindowWidth = 800;
float WindowHeight = 600;
RenderWindow window(VideoMode({ (unsigned int)WindowWidth, (unsigned int)WindowHeight }), "Feeding Frenzy 2");
View view({ 400.f, 300.f }, Vector2f(window.getSize()));

// =================================== Loading Screen =================================
Shader swayShader;
float deltaTime;
float startX = WindowWidth / 2.f;
float spacing = 65.0f;
float width_bubble_global;
float endTimer = 0.0f;
bool transitionStarted = false;
Vector2f ventPositions[2] = { {WindowWidth - 140.f, WindowHeight - 157.5f}, {WindowWidth - 220.f, WindowHeight - 117.f} };
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


// ==========================================
// ===== Select Level - Globals =====
// ==========================================

// bg / sign / banner
Texture bgTexture("C:/Users/MOHAMED/source/repos/test1/x64/Debug/Assets/Select_level/gamemap_bg.jpeg");
Sprite bgSprite(bgTexture);
Texture signTexture("C:/Users/MOHAMED/source/repos/test1/x64/Debug/Assets/Select_level/gamemap_tittleboard.jpeg");
Sprite signSprite(signTexture);
Texture bannerTexture("C:/Users/MOHAMED/source/repos/test1/x64/Debug/Assets/Select_level/bord.jpeg");
Sprite bannerSprite(bannerTexture);

// back button (normal / hover)
Texture myTexture("C:/Users/MOHAMED/source/repos/test1/x64/Debug/Assets/Select_level/shell_tinybtn124_normal.jpeg");
Texture myHoverTexture("C:/Users/MOHAMED/source/repos/test1/x64/Debug/Assets/Select_level/shell_tinybtn124_high.jpeg");
Sprite mySprite(myTexture);

// pearl textures (locked / unlocked) + load bar
Texture pearlTexture("C:/Users/MOHAMED/source/repos/test1/x64/Debug/Assets/Select_level/white.jpeg");
Texture pearlUnlockedTexture("C:/Users/MOHAMED/source/repos/test1/x64/Debug/Assets/Select_level/gold.jpeg");
Texture loadBarTexture("C:/Users/MOHAMED/source/repos/test1/x64/Debug/Assets/Select_level/gamemap_loadbar.jpeg");

// pearl sprites - normal & time attack
Sprite pearlSprite1(pearlTexture), pearlSprite2(pearlTexture), pearlSprite3(pearlTexture);
Sprite ta_pearlSprite1(pearlTexture), ta_pearlSprite2(pearlTexture), ta_pearlSprite3(pearlTexture);

// pearl positions & scales (locked / unlocked)
sf::Vector2f lockedPos1 = { 220.f, 87.f };   sf::Vector2f lockedScale1 = { 0.08f, 0.08f };
sf::Vector2f lockedPos2 = { 249.f, 133.f };  sf::Vector2f lockedScale2 = { 0.06f, 0.06f };
sf::Vector2f lockedPos3 = { 246.f, 170.f };  sf::Vector2f lockedScale3 = { 0.06f, 0.06f };

sf::Vector2f unlockedPos1 = { 235.f, 110.f }; sf::Vector2f unlockedScale1 = { 0.036f, 0.036f };
sf::Vector2f unlockedPos2 = { 261.f, 152.f }; sf::Vector2f unlockedScale2 = { 0.029f, 0.029f };
sf::Vector2f unlockedPos3 = { 255.f, 186.f }; sf::Vector2f unlockedScale3 = { 0.028f, 0.028f };

// UI texts
static sf::Font font("C:/Users/MOHAMED/source/repos/test1/x64/Debug/Assets/Fonts/ARIALNBI.ttf");
static sf::Text staticTxt(font, ""), nowLoadingTxt(font, ""), menuTxt(font, ""), levelTxt(font, ""), loadingtxt(font, "");

// level unlock state
bool level1Unlocked = true, level2Unlocked = false, level3Unlocked = false;
bool ta_level1Unlocked = false, ta_level2Unlocked = false, ta_level3Unlocked = false;
bool isTimeAttackMode = false;

// pearl data struct
struct PearlData
{
    sf::Sprite* sprite;
    float radius;
    std::string name;
    bool* unlocked;
    sf::Vector2f lockedPos, lockedScale, unlockedPos, unlockedScale;
};

PearlData pearls[3] = {
    {&pearlSprite1, 25.f, "Level 1", &level1Unlocked, lockedPos1, lockedScale1, unlockedPos1, unlockedScale1},
    {&pearlSprite2, 25.f, "Level 2", &level2Unlocked, lockedPos2, lockedScale2, unlockedPos2, unlockedScale2},
    {&pearlSprite3, 25.f, "Level 3", &level3Unlocked, lockedPos3, lockedScale3, unlockedPos3, unlockedScale3},
};
PearlData ta_pearls[3] = {
    {&ta_pearlSprite1, 25.f, "Level 1", &ta_level1Unlocked, lockedPos1, lockedScale1, unlockedPos1, unlockedScale1},
    {&ta_pearlSprite2, 25.f, "Level 2", &ta_level2Unlocked, lockedPos2, lockedScale2, unlockedPos2, unlockedScale2},
    {&ta_pearlSprite3, 25.f, "Level 3", &ta_level3Unlocked, lockedPos3, lockedScale3, unlockedPos3, unlockedScale3},
};

// ==========================================
// ===== Game Screen - Globals =====
// ==========================================

Texture gameScreenBgTexture;
Sprite gameScreenBgSprite(gameScreenBgTexture);
bool gameScreenActive = false;
int selectedLevel = -1;

// button positions & scales (options / quit / continue)
Texture gsBtn1NormalTex, gsBtn1HoverTex; optional<Sprite> gsBtn1Sprite;
const float GS_BTN1_X = 180.f, GS_BTN1_Y = 560.f, GS_BTN1_SCALE = 1 / 10.0f;

Texture gsBtn2NormalTex, gsBtn2HoverTex; optional<Sprite> gsBtn2Sprite;
const float GS_BTN2_X = 630.f, GS_BTN2_Y = 560.f, GS_BTN2_SCALE = 2 / 10.0f;

Texture gsBtn3NormalTex, gsBtn3HoverTex; optional<Sprite> gsBtn3Sprite;
const float GS_BTN3_X = 400.f, GS_BTN3_Y = 540.f;
const float GS_BTN3_SCALE = 1.5f / 10.f, GS_BTN3_HOVER_SCALE = 3.5f / 10.f;
const float GS_BTN3_HOVER_HALF_W = 80.f, GS_BTN3_HOVER_HALF_H = 30.f;

bool showQuitPopup = false;

// button struct (normal / hover, position, scale)
struct GSButton
{
    Texture normalTex, hoverTex;
    optional<Sprite> sprite;
    float x, y, scale, hoverScale;
    float hoverHalfW = 0.f, hoverHalfH = 0.f;
};
GSButton gsBtns[3];

// quit popup structs
struct QuitPopupButton { Texture normalTex, hoverTex; optional<Sprite> sprite; };
struct QuitPopup
{
    Texture bgTex, titleTex;
    optional<Sprite> bgSprite, titleSprite;
    QuitPopupButton btns[3];
};
QuitPopup quitPopup;

// level assets structs
struct LevelImage { Texture tex; optional<Sprite> sprite; float x, y, scale; };
struct LevelText { optional<Text> text; string str; int size; Color color; float x, y; bool centered; };
struct LevelData { Font font; LevelImage images[4]; int imageCount; LevelText texts[7]; int textCount; };
LevelData levels[3];

// quit popup texts & layout constants
Text quitPopupLine1(font, "", 22), quitPopupLine2(font, "", 18);
const float QUIT_POPUP_X = 400.f, QUIT_POPUP_Y = 330.f;
const float QUIT_BTN1_Y = 300.f, QUIT_BTN2_Y = 360.f, QUIT_BTN3_Y = 420.f;
const float QUIT_BTN_SCALE = 0.7f;


// --- تحميل الصور والأجسام ---
Texture Background_Loading("C:/Users/MOHAMED/source/repos/test1/x64/Debug/Assets/Main menu & Loading/Loading Screen/Images/loadbg.png");
Sprite Background_Loading_sprite(Background_Loading);
Texture Game_Icon("C:/Users/MOHAMED/source/repos/test1/x64/Debug/Assets/Main menu & Loading/Loading Screen/Images/LOGO.png");
Sprite Game_Icon_sprite(Game_Icon);
Texture POPCAP_Logo("C:/Users/MOHAMED/source/repos/test1/x64/Debug/Assets/Main menu & Loading/Loading Screen/Images/popCapLogo.png");
Sprite POPCAP_Logo_sprite(POPCAP_Logo);
Texture LogoWood("C:/Users/MOHAMED/source/repos/test1/x64/Debug/Assets/Main menu & Loading/Loading Screen/Images/sproutLogoWood.png");
Sprite LogoWood_sprite(LogoWood);
Texture loadBar("C:/Users/MOHAMED/source/repos/test1/x64/Debug/Assets/Main menu & Loading/Loading Screen/Images/loadBar.png");
Sprite loadBar_sprite(loadBar);
Texture loadBar_empty("C:/Users/MOHAMED/source/repos/test1/x64/Debug/Assets/Main menu & Loading/Loading Screen/Images/loadBarCap.png");
Sprite loadBar_empty_sprite(loadBar_empty);
Texture bubbleTexture("C:/Users/MOHAMED/source/repos/test1/x64/Debug/Assets/Main menu & Loading/Loading Screen/Images/bubbles_letters.png");
Sprite bubbleSprite(bubbleTexture);
Texture fontTexture("C:/Users/MOHAMED/source/repos/test1/x64/Debug/Assets/Fonts/LOADING.png");
Sprite s_L(fontTexture);
Sprite s_O(fontTexture);
Sprite s_A(fontTexture);
Sprite s_D(fontTexture);
Sprite s_I(fontTexture);
Sprite s_N(fontTexture);
Sprite s_G(fontTexture);
Texture bubbleSmallTex("C:/Users/MOHAMED/source/repos/test1/x64/Debug/Assets/Main menu & Loading/Loading Screen/Images/fx_bubble_d0.png");
Sprite bubbleSmallSprite(bubbleSmallTex);
Texture spark("C:/Users/MOHAMED/source/repos/test1/x64/Debug/Assets/Main menu & Loading/Loading Screen/Images/sparkleb0.png");
Sprite sparkSprite(spark);

// ===============================================================================
// =================================== Main Menu =================================
// ===============================================================================
Texture mainbackground("C:/Users/MOHAMED/source/repos/test1/x64/Debug/Assets/Main menu & Loading/Main Menu/mm_mainmenubg.jpg");
Sprite background(mainbackground);
Texture logo("C:/Users/MOHAMED/source/repos/test1/x64/Debug/Assets/Main menu & Loading/Main Menu/logo-removebg-preview.png");
Sprite logosp(logo);
Texture Greenfishswim("C:/Users/MOHAMED/source/repos/test1/x64/Debug/Assets/Fish/greenfish/greenfishs.png");
Texture Greenfisheat("C:/Users/MOHAMED/source/repos/test1/x64/Debug/Assets/Fish/greenfish/dd.png");
Texture Greenfishturn("C:/Users/MOHAMED/source/repos/test1/x64/Debug/Assets/Fish/greenfish/fishturning.png");
Sprite Greenfish(Greenfishswim);
float GFvelocityX_AXIS = -2;
float GFvelocityY_AXIS = -2;
int GFswim = 0;
int GFeat = 0;
int GFturn = 0;
float GFchangedir = -1;
int counter = 0;

Texture Barracudatex("C:/Users/MOHAMED/source/repos/test1/x64/Debug/Assets/Fish/Barracuda/Barracuda.png");
Sprite Barracuda(Barracudatex);
int BFcol = 0;
int BFrow = 0;
float BFvelocityX_AXIS = -2;
float BFvelocityY_AXIS = -2;
float BFchangedir = -1;

Texture startgametex("C:/Users/MOHAMED/source/repos/test1/x64/Debug/Assets/Main menu & Loading/Main Menu/mm_startgame_normal-removebg-preview.png");
Texture startgamepressed("C:/Users/MOHAMED/source/repos/test1/x64/Debug/Assets/Main menu & Loading/Main Menu/mm_startgame_high-removebg-preview.png");
Sprite startgamebutton(startgametex);

SoundBuffer buttonpressed("C:/Users/MOHAMED/source/repos/test1/x64/Debug/Assets/Music and Sounds/mouseover.ogg");
Sound buttonpressedsound(buttonpressed);

Music mainmenumusic("C:/Users/MOHAMED/source/repos/test1/x64/Debug/Assets/Music and Sounds/menuMusic.mp3");
bool isMusicEnabled = true;

Texture timeattacktex("C:/Users/MOHAMED/source/repos/test1/x64/Debug/Assets/Main menu & Loading/Main Menu/mm_timeattack_normal-removebg-preview.png");
Texture timeattackpressed("C:/Users/MOHAMED/source/repos/test1/x64/Debug/Assets/Main menu & Loading/Main Menu/mm_timeattack_high-removebg-preview.png");
Sprite timeattackbutton(timeattacktex);

Texture highscoretex("C:/Users/MOHAMED/source/repos/test1/x64/Debug/Assets/Main menu & Loading/Main Menu/mm_highscores_normal-removebg-preview.png");
Texture highscorepressed("C:/Users/MOHAMED/source/repos/test1/x64/Debug/Assets/Main menu & Loading/Main Menu/mm_highscores_high-removebg-preview.png");
Sprite highscorebutton(highscoretex);

Texture quittex("C:/Users/MOHAMED/source/repos/test1/x64/Debug/Assets/Main menu & Loading/Main Menu/mm_quit_normal-removebg-preview.png");
Texture quitpressed("C:/Users/MOHAMED/source/repos/test1/x64/Debug/Assets/Main menu & Loading/Main Menu/mm_quit_high-removebg-preview.png");
Sprite quitbutton(quittex);

Texture optionstex("C:/Users/MOHAMED/source/repos/test1/x64/Debug/Assets/Main menu & Loading/Main Menu/mm_options_normal-removebg-preview.png");
Texture optionspressed("C:/Users/MOHAMED/source/repos/test1/x64/Debug/Assets/Main menu & Loading/Main Menu/mm_options_high-removebg-preview.png");
Sprite optionsbutton(optionstex);

Texture switchusertex("C:/Users/MOHAMED/source/repos/test1/x64/Debug/Assets/Main menu & Loading/Main Menu/mm_switchuser_normal-removebg-preview.png");
Texture switchuserpressed("C:/Users/MOHAMED/source/repos/test1/x64/Debug/Assets/Main menu & Loading/Main Menu/mm_switchuser_high-removebg-preview.png");
Sprite switchuserbutton(switchusertex);

Texture creditstex("C:/Users/MOHAMED/source/repos/test1/x64/Debug/Assets/Main menu & Loading/Main Menu/mm_credits_normal-removebg-preview.png");
Texture creditspressed("C:/Users/MOHAMED/source/repos/test1/x64/Debug/Assets/Main menu & Loading/Main Menu/mm_credits_high-removebg-preview.png");
Sprite creditsbutton(creditstex);

Texture minowfishtex("C:/Users/MOHAMED/source/repos/test1/x64/Debug/Assets/Fish/herfish/herFish.png");
Sprite Minowfish(minowfishtex);
int MFcol = 0;
int MFrow = 0;
float MFchangedir = -1;
float MFvelocityX_AXIS = -2;
float MFvelocityY_AXIS = -2;
bool MFvisable = 1;
RectangleShape MFcollieder(Vector2f{ 57, 25 });

float smallfishsvelocityYaxis = getRandom(-2.f, 2.f);
CircleShape b({ 20 });

struct AnimatedObject
{
public:
    float changedir = 1;
    float velocityX_AXIS = -2;
    float velocityY_AXIS = -2;
    AnimatedObject(const Texture& texture, int frameWidth, int frameHeight)
        : sprite(texture, IntRect({ 0, 0 }, { frameWidth, frameHeight }))
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
        sprite.move({ velocityX_AXIS, velocityY_AXIS });
        changedir = (velocityX_AXIS > 0) ? -1.f : 1.f;
        sprite.setScale({ 0.2f * changedir, 0.2f });
        timer = 0.0f;
        sprite.setTextureRect(IntRect({ currentFrame * frameWidth, 0 }, { frameWidth, frameHeight }));
        currentFrame++;
        if (currentFrame == 15)
        {
            currentFrame = 0;
        }
    }
};
vector<AnimatedObject> smallfishs;

RectangleShape Barracudacollieder(Vector2f{ 271, 121 });
RectangleShape QueenTriggercollieder(Vector2f{ 260, 160 });
int cu = 0;
float totaltime = 0.0f;
Vector2i mouseLocalPos;
Vector2f mouseWorldPos;
CircleShape c({ 10 });

Texture QueenTrigerTEX("C:/Users/MOHAMED/source/repos/test1/x64/Debug/Assets/Fish/QueenTrigger/QueenTrigger.png");
Sprite QueenTrigger(QueenTrigerTEX);
int QTcol = 0;
int QTrow = 0;
float QTchangedir = -1;
float QTvelocityX_AXIS = -2;
float QTvelocityY_AXIS = -2;
RectangleShape n;
int num = 0;

bool playsound = 1;

//================================= Switch User ==================================
Texture FullTex;
Sprite Full(FullTex);
Texture TitleTex;
Sprite Title(TitleTex);
Texture NewButtonTex;
Sprite NewButton(NewButtonTex);
Texture NewButtonHLTex;
Sprite NewButtonHL(NewButtonHLTex);
Texture SelectButtonTex;
Sprite SelectButton(SelectButtonTex);
Texture SelectButtonHLTex;
Sprite SelectButtonHL(SelectButtonHLTex);
Texture DeleteButtonTex;
Sprite DeleteButton(DeleteButtonTex);
Texture DeleteButtonHLTex;
Sprite DeleteButtonHL(DeleteButtonHLTex);
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
Texture DoneAddingUserHLTex;
Sprite DoneAddingUserHL(DoneAddingUserHLTex);

struct Player
{
    string name = "";
    int id = 0;
};
Player players[7];
int NumberOfUsers = 0;
void SaveUsers()
{
    ofstream file("C:/Users/MOHAMED/source/repos/test1/x64/Debug/Assets/Switch User/Users_List.txt");
    for (int id = 0; id < NumberOfUsers; id++)
        file << players[id].name << '\t' << players[id].id << '\n';
    file.close();
}
const int MaxNumberOfUsers = 7;
int SelectedUser = -1;
Text* PlayersTexts[8];
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
Text DupplicateOKBtnText(btnFont, "", 40);
Text FullListOKBtnText(btnFont, "", 40);
Texture DupplicateBgTex;
Sprite DupplicateBg(DupplicateBgTex);
Text DupplicatedUserText(btnFont, "", 40);
Texture DupplicatedUserTex;
bool CamefromDupplicate = 0;



Texture quit_bg("C:/Users/MOHAMED/source/repos/test1/x64/Debug/Assets/Quit/bg_quit.png");
Sprite quit_bg_sprite(quit_bg);
Texture quit_button("C:/Users/MOHAMED/source/repos/test1/x64/Debug/Assets/Quit/Button.png");
Sprite quit_button_sprite(quit_button);
Texture quit_button_hover("C:/Users/MOHAMED/source/repos/test1/x64/Debug/Assets/Quit/Button High.png");
Sprite quit_button_hover_sprite(quit_button_hover);
Texture quit_icon("C:/Users/MOHAMED/source/repos/test1/x64/Debug/Assets/Quit/quitgame_icon.png");
Sprite quit_icon_sprite(quit_icon);

bool quit_yes_pressed = false;
bool quit_no_pressed = false;

Text quit_yes_text(btnFont, "Yes", 30);
Text quit_no_text(btnFont, "No!", 30);
Text quit_title_text(btnFont, "Are you sure you want to quit", 30);
Text quit_title_text2(btnFont, "Feeding Frenzy 2?", 30);

// =====================================================================
// SECTION 1: CORE CONFIGURATION & LAYOUT SETTINGS
// =====================================================================
const float LAYOUT_LABEL_X = 370.f;
const float LAYOUT_BUBBLE_START_X = 390.f;
const float LAYOUT_ROW_SPACING = 45.f;
const float LAYOUT_COL_SPACING = 43.f;
const float POS_Y_TOP_OPTIONS = 170.f;
const float POS_Y_MOUSE_SPEED = 310.f;
const float POS_Y_GAME_DETAIL = 360.f;

// =====================================================================
// SECTION 2: GLOBAL ENGINE VARIABLES
// =====================================================================
SoundBuffer WaterSound;
optional<Sound> waterloop;
const int NUM_SFX_BUFFERS = 4;
SoundBuffer sfxBuffers[NUM_SFX_BUFFERS];
const int NUM_SOUND_CHANNELS = 5;
optional<Sound> sfxChannels[NUM_SOUND_CHANNELS];
Clock sfxTimer;
float nextSfxDelay = 0.f;

// =====================================================================
// SECTION 3: ASSETS & UI STATE
// =====================================================================
Font globalFont;
RectangleShape screenDarkener;

Texture texBgMain, texBgPlank, texTitle, texCorals;
optional<Sprite> sprBgMain, sprBgPlank, sprTitle, sprCorals;

optional<Text> mouseLabel, detailLabel, versionLabel;

Texture texUncheckedNormal, texUncheckedHover;
Texture texCheckedNormal, texCheckedHover;
Texture texDoneNormal, texDoneHover;

struct MenuOption
{
    optional<Text> text;
    optional<Sprite> checkbox;
    bool isChecked = false;
    bool isHovered = false;
    bool isCheckbox = true;
};
const int NumOptions = 11;
MenuOption OptionButtons[NumOptions];
int MSpeedIndex = 1;
int GraphicsIndex = 2;
bool prevMousePressed = false;
bool shouldCloseOptions = false;

float barX = 258.f;
float barY = 495.f;
float barWidth = 308.f;
float barHeight = 28.f;
float barSpeed = 150.f;
bool isLoading = false;
float loadProgress = 0.f;

RectangleShape fadeRect;
Clock fadeClock;
bool isFadingOut = false;
bool isFadingIn = false;
float fadeAlpha = 0.f;

// highscore
struct HighScoreEntry
{
    string name;
    int score;
};

const int MAX_SCORES = 25;
const int VISIBLE_SCORES = 10;
HighScoreEntry story_scores[MAX_SCORES];
HighScoreEntry timeattack_scores[MAX_SCORES];

void proceduralSort(HighScoreEntry arr[]);

// ================================= High Score =================================
Texture texHSDoneNormal("C:/Users/MOHAMED/source/repos/test1/x64/Debug/Assets/Highscore/done_normal.png");
Texture texHSDoneHover("C:/Users/MOHAMED/source/repos/test1/x64/Debug/Assets/Highscore/done_hover.png");
Sprite sprHSDonePlank(texHSDoneNormal);

Texture texHSResetNormal("C:/Users/MOHAMED/source/repos/test1/x64/Debug/Assets/Highscore/shell_tinybtn124_normal.png");
Texture texHSResetHover("C:/Users/MOHAMED/source/repos/test1/x64/Debug/Assets/Highscore/shell_tinybtn124_high.jpg");
Sprite sprHSResetPlank(texHSResetNormal);

Texture texHSArrowNormal("C:/Users/MOHAMED/source/repos/test1/x64/Debug/Assets/Highscore/_sidescrollbtn.png");
Texture texHSArrowHover("C:/Users/MOHAMED/source/repos/test1/x64/Debug/Assets/Highscore/_sidescrollbtnhi.png");
Texture texHSArrowDown("C:/Users/MOHAMED/source/repos/test1/x64/Debug/Assets/Highscore/_sidescrollbtndown.png");
Sprite sprHSStoryArrowRight(texHSArrowNormal);
Sprite sprHSStoryArrowLeft(texHSArrowNormal);
Sprite sprHSListArrowUp(texHSArrowNormal);
Sprite sprHSListArrowDown(texHSArrowNormal);

Font fontHSTitle("C:/Users/MOHAMED/source/repos/test1/x64/Debug/Assets/Fonts/BernardMT.ttf");
Font fontHSMain("C:/Users/MOHAMED/source/repos/test1/x64/Debug/Assets/Fonts/Barmeno.ttf");
Font fontHSDone("C:/Users/MOHAMED/source/repos/test1/x64/Debug/Assets/Fonts/BarmenoBold.otf");

Text textHSMainTitle(fontHSTitle, "High Scores", 48);
Text textHSStoryMode(fontHSMain, "Story Mode", 22);
Text textHSDoneButton(fontHSDone, "Done", 30);
Text textHSResetButton(fontHSMain, "Reset All Scores", 16);

Text textHSListRanks[VISIBLE_SCORES] = {
    {fontHSMain, "", 20}, {fontHSMain, "", 20}, {fontHSMain, "", 20}, {fontHSMain, "", 20}, {fontHSMain, "", 20},
    {fontHSMain, "", 20}, {fontHSMain, "", 20}, {fontHSMain, "", 20}, {fontHSMain, "", 20}, {fontHSMain, "", 20} };
Text textHSListNames[VISIBLE_SCORES] = {
    {fontHSMain, "", 20}, {fontHSMain, "", 20}, {fontHSMain, "", 20}, {fontHSMain, "", 20}, {fontHSMain, "", 20},
    {fontHSMain, "", 20}, {fontHSMain, "", 20}, {fontHSMain, "", 20}, {fontHSMain, "", 20}, {fontHSMain, "", 20} };
Text textHSListScores[VISIBLE_SCORES] = {
    {fontHSMain, "", 20}, {fontHSMain, "", 20}, {fontHSMain, "", 20}, {fontHSMain, "", 20}, {fontHSMain, "", 20},
    {fontHSMain, "", 20}, {fontHSMain, "", 20}, {fontHSMain, "", 20}, {fontHSMain, "", 20}, {fontHSMain, "", 20} };

RectangleShape HSModeUnderline;
enum GameMode
{
    STORY,
    TIME_ATTACK
};
GameMode currentMode = STORY;
int scrollOffset = 0;

const float LIST_START_Y = 165.f;
const float LIST_SPACING = 28.f;

const Color colorTitleGreen(165, 220, 75);
const Color colorListText(240, 240, 240);
const Color colorDoneText(180, 255, 100);
const Color colorResetText(255, 255, 150);
const Color colorHoverHighlight(255, 255, 0);

// credits
Font fontcredits("C:/Users/MOHAMED/source/repos/test1/x64/Debug/Assets/Fonts/trebuc.ttf");
optional<Text> creditsText;
Text textHSDoneButtoncredits(fontHSDone, "Done", 30);
Texture texHSDoneNormalcredits("C:/Users/MOHAMED/source/repos/test1/x64/Debug/Assets/Credits/done_normal.png");
Texture texHSDoneHovercredits("C:/Users/MOHAMED/source/repos/test1/x64/Debug/Assets/Credits/done_hover.png");
Sprite sprHSDonePlankcredits(texHSDoneNormalcredits);

int main()
{
    cout << "SFML 3.0 and Standard Library are working!" << endl;

    window.create(VideoMode::getDesktopMode(), "Feeding Frenzy 2", State::Fullscreen);

    StartLoadingScreen();
    Clock clock;
    Clock totalClock;

    fadeRect.setSize({ WindowWidth, WindowHeight });
    fadeRect.setFillColor(Color(0, 0, 0, 0));
    fadeRect.setPosition({ 0.f, 0.f });

    view.setSize({ 800.f, 600.f });
    view.setCenter({ 400.f, 300.f });
    view.setViewport(sf::FloatRect({ 0.f, 0.f }, { 1.f, 1.f }));

    window.setView(view);
    window.setFramerateLimit(60);

    while (window.isOpen())
    {
        deltaTime = clock.restart().asSeconds();
        float totalTime = totalClock.getElapsedTime().asSeconds();
        while (const optional event = window.pollEvent())
        {
            if (event->is<Event::Closed>())
                window.close();
            else if (const auto* resizeEvent = event->getIf<Event::Resized>())
            {
                view.setViewport(sf::FloatRect({ 0.f, 0.f }, { 1.f, 1.f }));
            }
        }
        window.setView(view);
        window.clear(Color::Black);
        if (DrawLoadingScreen(totalTime))
            break;
    }

    MainMenu();

    window.setFramerateLimit(30);
    while (window.isOpen())
    {
        deltaTime = clock.restart().asSeconds();
        while (const optional event = window.pollEvent())
        {
            if (event->is<Event::Closed>())
                window.close();
            if (event->is<Event::KeyPressed>())
                if (event->getIf<Event::KeyPressed>()->code == Keyboard::Key::Escape)
                    window.close();
        }
        window.setView(view);
    }
    return 0;
}

void loading_screen()
{
    window.setFramerateLimit(45);
    if (!swayShader.loadFromFile("sway.frag", Shader::Type::Fragment))
    {
        cout << "Error loading shader!" << endl;
    }
    if (!Background_Loading.loadFromFile("C:/Users/MOHAMED/source/repos/test1/x64/Debug/Assets/Main menu & Loading/Loading Screen/Images/loadbg.png"))
    {
        cout << "!!! Error: Could not find loadbg.png !!!" << endl;
    }
    Background_Loading_sprite.setTexture(Background_Loading);
    Background_Loading_sprite.setPosition({ 0, 0 });
    Vector2u texture_Size = Background_Loading.getSize();
    float ScaleX = WindowWidth / texture_Size.x;
    float ScaleY = WindowHeight / texture_Size.y;
    Background_Loading_sprite.setScale({ ScaleX, ScaleY });
    Background_Loading.setSmooth(true);
    if (!Game_Icon.loadFromFile("C:/Users/MOHAMED/source/repos/test1/x64/Debug/Assets/Main menu & Loading/Loading Screen/Images/LOGO.png"))
    {
        cout << "!!! Error: Could not find LOGO.png !!!" << endl;
    }
    float width = 300;
    float height = 150;
    Game_Icon_sprite.setTexture(Game_Icon);
    Game_Icon_sprite.setPosition({ (WindowWidth / 2) - 155, 7 });
    texture_Size = Game_Icon.getSize();
    ScaleX = width / Game_Icon.getSize().x;
    ScaleY = height / Game_Icon.getSize().y;
    Game_Icon_sprite.setScale({ ScaleX, ScaleY });
    Game_Icon.setSmooth(true);
    if (!POPCAP_Logo.loadFromFile("C:/Users/MOHAMED/source/repos/test1/x64/Debug/Assets/Main menu & Loading/Loading Screen/Images/popCapLogo.png"))
    {
        cout << "!!! Error: Could not find popCapLogo.png !!!" << endl;
    }
    width = 120;
    height = 110;
    POPCAP_Logo_sprite.setTexture(POPCAP_Logo);
    POPCAP_Logo_sprite.setPosition({ (WindowWidth - 155), WindowHeight - 126.f });
    texture_Size = POPCAP_Logo.getSize();
    ScaleX = width / POPCAP_Logo.getSize().x;
    ScaleY = height / POPCAP_Logo.getSize().y;
    POPCAP_Logo_sprite.setScale({ ScaleX, ScaleY });
    POPCAP_Logo.setSmooth(true);
    if (!LogoWood.loadFromFile("C:/Users/MOHAMED/source/repos/test1/x64/Debug/Assets/Main menu & Loading/Loading Screen/Images/sproutLogoWood.png"))
    {
        cout << "!!! Error: Could not find sproutLogoWood.png !!!" << endl;
    }
    width = 160;
    height = 80;
    LogoWood_sprite.setTexture(LogoWood);
    LogoWood_sprite.setPosition({ (0 + 25), WindowHeight - 115.f });
    texture_Size = LogoWood.getSize();
    ScaleX = width / LogoWood.getSize().x;
    ScaleY = height / LogoWood.getSize().y;
    LogoWood_sprite.setScale({ ScaleX, ScaleY });
    LogoWood.setSmooth(true);
    if (!fontTexture.loadFromFile("C:/Users/MOHAMED/source/repos/test1/x64/Debug/Assets/Fonts/LOADING.png"))
    {
        cout << "!!! Error: Could not find LOADING.png !!!" << endl;
    }
    fontTexture.setSmooth(true);
    float cellWidth = 19.4;
    float cellHeight = 35.5;
    Color targetColor(160, 211, 74);
    s_L.setTexture(fontTexture);
    s_L.setTextureRect(IntRect({ (int)(1 * cellWidth), (int)(3 * cellHeight) }, { (int)cellWidth, (int)cellHeight }));
    s_L.setColor(targetColor);
    s_L.setOrigin({ cellWidth / 2.f, cellHeight / 2.f });
    s_O.setTexture(fontTexture);
    s_O.setTextureRect(IntRect({ (int)(4 * 20), (int)(3 * 35.7) }, { 20, (int)35.7 }));
    s_O.setColor(targetColor);
    s_O.setOrigin({ 10.f, 35.7f / 2.f });
    s_A.setTexture(fontTexture);
    s_A.setTextureRect(IntRect({ (int)(3 * 20), (int)(2 * cellHeight) }, { 20, (int)cellHeight }));
    s_A.setColor(targetColor);
    s_A.setOrigin({ 10.f, cellHeight / 2.f });
    s_D.setTexture(fontTexture);
    s_D.setTextureRect(IntRect({ (int)(6 * 20), (int)(2 * cellHeight) }, { 20, (int)cellHeight }));
    s_D.setColor(targetColor);
    s_D.setOrigin({ 10.f, cellHeight / 2.f });
    s_I.setTexture(fontTexture);
    s_I.setTextureRect(IntRect({ (int)(10.39 * 20 + 4), (int)(2 * cellHeight) }, { 12, (int)cellHeight }));
    s_I.setColor(targetColor);
    s_I.setOrigin({ 6.f, cellHeight / 2.f });
    s_N.setTexture(fontTexture);
    s_N.setTextureRect(IntRect({ (int)(3 * 20), (int)(3 * cellHeight) }, { 20, (int)cellHeight }));
    s_N.setColor(targetColor);
    s_N.setOrigin({ 10.f, cellHeight / 2.f });
    s_G.setTexture(fontTexture);
    s_G.setTextureRect(IntRect({ 174, (int)(2 * cellHeight) }, { 18, (int)cellHeight }));
    s_G.setColor(targetColor);
    s_G.setOrigin({ 9.f, cellHeight / 2.f });
    bubbleSprite.setTexture(bubbleTexture);
    bubbleSprite.setScale({ 0.7f, 0.7f });
    bubbleSprite.setOrigin({ bubbleSprite.getLocalBounds().size.x / 2.f, bubbleSprite.getLocalBounds().size.y / 2.f });
    width_bubble_global = bubbleSprite.getGlobalBounds().size.x;
    bubbleTexture.setSmooth(true);
    width = 320.f;
    height = 50.f;
    loadBar_empty_sprite.setTexture(loadBar_empty);
    loadBar_empty_sprite.setOrigin({ loadBar_empty_sprite.getLocalBounds().size.x / 2.f, loadBar_empty_sprite.getLocalBounds().size.y / 2.f });
    loadBar_empty_sprite.setPosition({ WindowWidth / 2.f, WindowHeight - 85.f });
    texture_Size = loadBar_empty.getSize();
    ScaleX = width / loadBar_empty.getSize().x;
    ScaleY = height / loadBar_empty.getSize().y;
    loadBar_empty_sprite.setScale({ ScaleX, ScaleY });
    loadBar_empty.setSmooth(true);
    loadBar_sprite.setTexture(loadBar);
    loadBar_sprite.setOrigin({ loadBar_sprite.getLocalBounds().size.x / 2.f, loadBar_sprite.getLocalBounds().size.y / 2.f });
    loadBar_sprite.setPosition({ WindowWidth / 2.f, WindowHeight - 85.f });
    loadBar_sprite.setScale({ width / loadBar.getSize().x, height / loadBar.getSize().y });
    loadBar.setSmooth(true);
    bubbleSmallTex.setSmooth(true);
    spark.setSmooth(true);
}

void StartLoadingScreen()
{
    loading_screen();
}

void UpdateLoadingScreen()
{
}

bool DrawLoadingScreen(float totalTime)
{
    window.clear(Color::Black);
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
    loadBar_sprite.setTextureRect(IntRect({ 0, 0 }, { currentWidth, (int)textureSize.y }));
    window.draw(loadBar_empty_sprite);
    window.draw(loadBar_sprite);
    startX = WindowWidth / 2.0f - (3.5f * spacing) + 20.f;
    Sprite* letters[] = { &s_L, &s_O, &s_A, &s_D, &s_I, &s_N, &s_G };
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
        float curX = startX + (i * spacing);
        float curY = (WindowHeight / 2.0f - 40.f) + sin(totalTime * 4.0f + i * 0.8f) * 12.0f;
        bubbleSprite.setScale({ 0.7f * popScale, 0.7f * popScale });
        letters[i]->setScale({ 0.92f * popScale, 0.92f * popScale });
        bubbleSprite.setPosition({ curX, curY });
        letters[i]->setPosition({ curX, curY });
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
            float pX = startX + (idx * spacing);
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
                    sparkSprite.setPosition({ sparks[i].x, sparks[i].y });
                    sparkSprite.setColor(Color(255, 255, 255, (uint8_t)sparks[i].alpha));
                    sparkSprite.setScale({ 0.55f, 0.55f });
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
                    bubbleSmallSprite.setPosition({ letterBubbles[i].x, letterBubbles[i].y });
                    bubbleSmallSprite.setColor(Color(255, 255, 255, (uint8_t)letterBubbles[i].alpha));
                    window.draw(bubbleSmallSprite);
                }
            }
        }
        if (!anyActive)
        {
            particlesFinished = true;
        }
    }
    timerLeft += deltaTime;
    timerRight += deltaTime;
    bubbleTimer += deltaTime;
    auto spawnBubble = [&](int ventIdx) {
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
                ventBubbles[i].x = 810.f;
                ventBubbles[i].y = (float)(rand() % 601);
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
                bubbleSmallSprite.setPosition({ ventBubbles[i].x, ventBubbles[i].y });
                bubbleSmallSprite.setColor(Color(255, 255, 255, (uint8_t)ventBubbles[i].alpha));
                float s = ventBubbles[i].isVent ? (0.8f + (i % 3) * 0.2f) : 0.65f;
                bubbleSmallSprite.setScale({ s, s });
                window.draw(bubbleSmallSprite);
            }
        }
    }
    if (particlesFinished)
    {
        if (!transitionStarted)
        {
            transitionStarted = true;
            fadeClock.restart();
            fadeAlpha = 0.f;
        }
        fadeAlpha = std::min(255.f, fadeClock.getElapsedTime().asSeconds() * 800.f);
        fadeRect.setFillColor(Color(0, 0, 0, (uint8_t)fadeAlpha));
        window.draw(fadeRect);
        window.display();
        if (fadeAlpha >= 255.f)
            return true;
        return false;
    }
    window.display();
    return false;
}

void MainMenu()
{
    view.setSize({ WindowWidth, WindowHeight });
    view.setCenter({ WindowWidth / 2.f, WindowHeight / 2.f });
    view.setViewport(FloatRect({ 0.f, 0.f }, { 1.f, 1.f }));
    window.setView(view);
    StartMainMenu();
    FadeInFromBlack();
    Clock frameClock;
    while (window.isOpen())
    {
        deltaTime = frameClock.restart().asSeconds();
        totaltime += deltaTime;
        while (const optional event = window.pollEvent())
        {
            if (event->is<Event::Closed>() || Keyboard::isKeyPressed(Keyboard::Key::Escape))
                window.close();
            if (auto mouseEvent = event->getIf<Event::MouseButtonReleased>())
            {
                if (mouseEvent->button == Mouse::Button::Left)
                {
                    Vector2f mousePos = window.mapPixelToCoords(Mouse::getPosition(window), view);
                    if (startgamebutton.getGlobalBounds().contains(mousePos)) {
                        isTimeAttackMode = false;
                        FadeOutToBlack();
                        Select_level();
                        FadeInFromBlack();
                    }
                    if (timeattackbutton.getGlobalBounds().contains(mousePos)) {
                        isTimeAttackMode = true;
                        FadeOutToBlack();
                        Select_level();
                        FadeInFromBlack();
                    }
                    if (switchuserbutton.getGlobalBounds().contains(mousePos))
                    {
                        FadeOutToBlack();
                        SwitchUser();
                        FadeInFromBlack();
                    }
                    if (optionsbutton.getGlobalBounds().contains(mousePos))
                    {
                        FadeOutToBlack();
                        OptionsMenu();
                        FadeInFromBlack();
                    }
                    if (quitbutton.getGlobalBounds().contains(mousePos))
                    {
                        FadeOutToBlack();
                        QuitGame();
                        FadeInFromBlack();
                    }
                    if (highscorebutton.getGlobalBounds().contains(mousePos))
                    {
                        FadeOutToBlack();
                        Highscore();
                        FadeInFromBlack();
                    }
                    if (creditsbutton.getGlobalBounds().contains(mousePos))
                    {
                        FadeOutToBlack();
                        Credits();
                        FadeInFromBlack();
                    }
                }
            }
            else if (const auto* resizeEvent = event->getIf<Event::Resized>())
            {
                view.setViewport(sf::FloatRect({ 0.f, 0.f }, { 1.f, 1.f }));
            }
        }
        window.setView(view);
        background.setTexture(mainbackground);
        UpdateMainMenu();
        DrawMainMenu();
    }
}

void StartMainMenu()
{
    if (!swayShader.loadFromFile("sway.frag", Shader::Type::Fragment))
    {
        cout << "Error loading shader!" << endl;
    }
    smallfishs.clear();
    if (isMusicEnabled && mainmenumusic.getStatus() != Music::Status::Playing)
        mainmenumusic.play();
    mainmenumusic.setLooping(true);
    srand(time(0));
    for (int i = 0; i < 8; i++)
    {
        AnimatedObject obj{ minowfishtex, 286, 126 };
        float speedX = getRandom(1.5f, 3.f);
        float speedY = getRandom(0.5f, 2.f);
        obj.velocityX_AXIS = (rand() % 2 == 0) ? speedX : -speedX;
        obj.velocityY_AXIS = (rand() % 2 == 0) ? speedY : -speedY;
        if (i == 2 || i == 4 || i == 6)
        {
            obj.velocityY_AXIS = -obj.velocityY_AXIS;
        }
        obj.shape.setFillColor(Color::Green);
        obj.shape.setSize(Vector2f{ 57, 25 });
        obj.shape.setOrigin(obj.shape.getLocalBounds().size / 2.f);
        obj.sprite.setTexture(minowfishtex);
        obj.sprite.setOrigin({ 285 / 2, 126 / 2 });
        obj.sprite.setPosition(Vector2f({ WindowWidth / 2.0f + (i * 100), WindowHeight / 2.0f + (i * rand() % 100) }));
        obj.changedir = (obj.velocityX_AXIS > 0) ? -1.f : 1.f;
        obj.sprite.setScale({ 0.2f * obj.changedir, 0.2f });
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
    background.setOrigin(background.getLocalBounds().size / 2.0f);
    background.setPosition(Vector2f({ WindowWidth / 2.0f, WindowHeight / 2.0f }));
    logosp.setOrigin(logosp.getLocalBounds().size / 2.0f);
    logosp.setPosition(Vector2f({ WindowWidth / 2.0f, WindowHeight / 2.0f - 200 }));
    Greenfish.setPosition(Vector2f({ WindowWidth / 2.0f - 300, WindowHeight / 2.0f }));
    startgamebutton.setOrigin(startgamebutton.getLocalBounds().size / 2.0f);
    startgamebutton.setPosition(Vector2f({ WindowWidth / 2.0f, WindowHeight / 2.0f - 50 }));
    timeattackbutton.setOrigin(timeattackbutton.getLocalBounds().size / 2.0f);
    timeattackbutton.setPosition(Vector2f({ WindowWidth / 2.0f + 10, WindowHeight / 2.0f + 50 }));
    highscorebutton.setOrigin(highscorebutton.getLocalBounds().size / 2.0f);
    highscorebutton.setPosition(Vector2f({ WindowWidth / 2.0f + -150, WindowHeight / 2.0f + 140 }));
    optionsbutton.setOrigin(optionsbutton.getLocalBounds().size / 2.0f);
    optionsbutton.setPosition(Vector2f({ WindowWidth / 2.0f + 120, WindowHeight / 2.0f + 150 }));
    quitbutton.setOrigin(quitbutton.getLocalBounds().size / 2.0f);
    quitbutton.setPosition(Vector2f({ WindowWidth / 2.0f, WindowHeight / 2.0f + 230 }));
    switchuserbutton.setOrigin(switchuserbutton.getLocalBounds().size / 2.0f);
    switchuserbutton.setPosition(Vector2f({ WindowWidth / 2.0f + 300, WindowHeight / 2.0f + 250 }));
    creditsbutton.setOrigin(creditsbutton.getLocalBounds().size / 2.0f);
    creditsbutton.setPosition(Vector2f({ WindowWidth / 2.0f - 300, WindowHeight / 2.0f + 240 }));
    Barracuda.setPosition(Vector2f({ WindowWidth / 2.f - 700, WindowHeight / 2.f - 200 }));
    QueenTrigger.setPosition({ 1450, 360 });
}

void UpdateMainMenu()
{
    float smallfishsvelocityYaxis = getRandom(-2.f, 2.f);
    if (Barracuda.getScale().x == -1)
    {
        Barracudacollieder.setPosition(Vector2f(Barracuda.getPosition().x + 80, Barracuda.getPosition().y + 35));
    }
    else
    {
        Barracudacollieder.setPosition(Vector2f(Barracuda.getPosition().x - 80, Barracuda.getPosition().y + 35));
    }
    QueenTriggercollieder.setPosition(Vector2f(QueenTrigger.getPosition().x, QueenTrigger.getPosition().y));
    MFcollieder.setPosition(Vector2f(Minowfish.getPosition().x, Minowfish.getPosition().y));
    Vector2i mouseLocalPos = Mouse::getPosition(window);
    Vector2f mouseWorldPos = window.mapPixelToCoords(mouseLocalPos);
    for (auto& obj : smallfishs)
    {
        obj.shape.setPosition({ obj.sprite.getPosition().x, obj.sprite.getPosition().y });
        obj.update(286, 126);
        float posX = obj.sprite.getPosition().x;
        float posY = obj.sprite.getPosition().y;
        if (posX <= -150.f || posX >= WindowWidth + 150.f)
        {
            obj.velocityX_AXIS *= -1;
            if (posX <= -150.f)
                obj.sprite.setPosition({ -149.f, posY });
            else
                obj.sprite.setPosition({ WindowWidth + 149.f, posY });
        }
        if (posY <= -100.f || posY >= WindowHeight + 100.f)
        {
            obj.velocityY_AXIS *= -1;
            if (posY <= -100.f)
                obj.sprite.setPosition({ posX, -99.f });
            else
                obj.sprite.setPosition({ posX, WindowHeight + 99.f });
        }
    }
    PlayingSound(true);
    ChangingButtonShape();
    BarracudaFishanimation();
    QueenTriggerFish();
}

void ChangingButtonShape()
{
    Vector2i mouseLocalPos = Mouse::getPosition(window);
    Vector2f mouseWorldPos = window.mapPixelToCoords(mouseLocalPos, view);
    if (startgamebutton.getGlobalBounds().contains(mouseWorldPos))
        startgamebutton.setTexture(startgamepressed);
    else
        startgamebutton.setTexture(startgametex);
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

void DrawMainMenuBackground()
{
    window.setView(view);
    window.clear();
    swayShader.setUniform("time", totaltime);
    window.draw(background, &swayShader);
    window.draw(QueenTrigger);
    for (auto& obj : smallfishs)
        window.draw(obj.sprite);
    window.draw(Barracuda);
}

void DrawMainMenu()
{
    window.setView(view);
    DrawMainMenuBackground();
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

void MainMenuFishAnimation()
{
    GreenfishAnimation();
    MinowFishanimation();
    BarracudaFishanimation();
    QueenTriggerFish();
}

void GreenfishAnimation()
{
    Greenfish.move({ GFvelocityX_AXIS, GFvelocityY_AXIS });
    if (Greenfish.getPosition().x == window.getSize().x / 2.0f - 580 || Greenfish.getPosition().x == window.getSize().x / 2.0f + 580)
    {
        counter = 0;
        while (true)
        {
            Greenfish.setTexture(Greenfishturn);
            Greenfish.setTextureRect(IntRect({ GFturn * 227, 0 }, { 227, 255 }));
            GFturn++;
            if (GFturn == 13)
            {
                GFturn = 0;
                Greenfish.setScale({ GFchangedir, 1 });
                GFvelocityX_AXIS = GFvelocityX_AXIS * -1;
                break;
            }
        }
        GFchangedir *= -1;
    }
    else
    {
        Greenfish.setTexture(Greenfishswim);
        Greenfish.setTextureRect(IntRect({ GFswim * 227, 0 }, { 227, 233 }));
        GFswim = (GFswim + 1) % 13;
    }
    if (Greenfish.getGlobalBounds().findIntersection(Minowfish.getGlobalBounds()))
    {
        if (counter < 1)
        {
            GFeat = 0;
        }
        if (counter == 4)
        {
            Minowfish.setPosition({ 3000, 3000 });
        }
        counter++;
        if (counter <= 5)
        {
            Greenfish.setTexture(Greenfisheat);
            Greenfish.setTextureRect(IntRect({ GFeat * 227, 0 }, { 227, 233 }));
            GFeat++;
        }
        else
        {
            Greenfish.setTexture(Greenfishswim);
            Greenfish.setTextureRect(IntRect({ GFswim * 227, 0 }, { 227, 233 }));
            GFswim = (GFswim + 1) % 13;
        }
    }
}

void PlayingSound(bool isMainMenu)
{
    Vector2i mouseLocalPos = Mouse::getPosition(window);
    Vector2f mouseWorldPos = window.mapPixelToCoords(mouseLocalPos);
    bool isHovering = false;
    if (isMainMenu)
    {
        isHovering = (startgamebutton.getGlobalBounds().contains(mouseWorldPos) ||
            timeattackbutton.getGlobalBounds().contains(mouseWorldPos) ||
            highscorebutton.getGlobalBounds().contains(mouseWorldPos) ||
            optionsbutton.getGlobalBounds().contains(mouseWorldPos) ||
            quitbutton.getGlobalBounds().contains(mouseWorldPos) ||
            switchuserbutton.getGlobalBounds().contains(mouseWorldPos) ||
            creditsbutton.getGlobalBounds().contains(mouseWorldPos) ||
            YesButton.getGlobalBounds().contains(mouseWorldPos) ||
            NoButton.getGlobalBounds().contains(mouseWorldPos) ||
            sprHSDonePlankcredits.getGlobalBounds().contains(mouseWorldPos) ||
            sprHSResetPlank.getGlobalBounds().contains(mouseWorldPos));
    }
    else
    {
        isHovering = NewButton.getGlobalBounds().contains(mouseWorldPos) ||
            SelectButton.getGlobalBounds().contains(mouseWorldPos) ||
            DeleteButton.getGlobalBounds().contains(mouseWorldPos) ||
            quit_yes_text.getGlobalBounds().contains(mouseWorldPos) ||
            quit_no_text.getGlobalBounds().contains(mouseWorldPos) ||
            mySprite.getGlobalBounds().contains(mouseWorldPos) ||
            (OptionButtons[10].checkbox.has_value() && OptionButtons[10].checkbox->getGlobalBounds().contains(mouseWorldPos)) ||
            YesButton.getGlobalBounds().contains(mouseWorldPos) ||
            NoButton.getGlobalBounds().contains(mouseWorldPos) ||
            sprHSDonePlankcredits.getGlobalBounds().contains(mouseWorldPos) ||
            sprHSResetPlank.getGlobalBounds().contains(mouseWorldPos);
    }
    if (isHovering)
    {
        if (playsound)
        {
            buttonpressedsound.play();
            playsound = 0;
        }
    }
    else
    {
        playsound = 1;
    }
}

void MinowFishanimation()
{
    if (MFvisable)
    {
        Minowfish.move({ MFvelocityX_AXIS, 0 });
        if (Minowfish.getPosition().x == window.getSize().x / 2.0f - 580 || Minowfish.getPosition().x == window.getSize().x / 2.0f + 580)
        {
            while (true)
            {
                Minowfish.setTextureRect(IntRect({ MFcol * 286, 1 * 126 }, { 286, 126 }));
                if (MFcol == 6)
                {
                    Minowfish.setScale({ float(0.2 * MFchangedir), 0.2 });
                    MFvelocityX_AXIS = MFvelocityX_AXIS * -1;
                    break;
                }
                MFcol++;
            }
            MFchangedir *= -1;
        }
        else
        {
            Minowfish.setTextureRect(IntRect({ MFcol * 286, 0 }, { 286, 126 }));
            MFcol = (MFcol + 1) % 15;
        }
    }
}

void BarracudaFishanimation()
{
    Barracuda.move({ BFvelocityX_AXIS, 0 });
    BFrow = 0;
    if (Barracuda.getPosition().x == window.getSize().x / 2.0f - 580 || Barracuda.getPosition().x == window.getSize().x / 2.0f + 580)
    {
        counter = 0;
        BFcol = 0;
        while (true)
        {
            Barracuda.setTextureRect(IntRect({ BFcol * 256, 2 * 118 }, { 256, 118 }));
            BFcol++;
            if (BFcol == 5)
            {
                BFcol = 0;
                Barracuda.setScale({ BFchangedir, 1 });
                BFvelocityX_AXIS = BFvelocityX_AXIS * -1;
                break;
            }
        }
        BFchangedir *= -1;
    }
    else if (Barracudacollieder.getGlobalBounds().findIntersection(MFcollieder.getGlobalBounds()))
    {
        if (counter < 1)
        {
            BFcol = 0;
        }
        if (counter == 4)
        {
            Minowfish.setPosition({ 3000, 3000 });
        }
        counter++;
        if (counter <= 6)
        {
            Barracuda.setTextureRect(IntRect({ BFcol * 256, 0 }, { 256, 118 }));
            BFcol++;
        }
        else
        {
            Barracuda.setTextureRect(IntRect({ BFcol * 256, 1 * 118 }, { 256, 118 }));
            BFcol = (BFcol + 1) % 14;
        }
    }
    else
    {
        Barracuda.setTextureRect(IntRect({ BFcol * 256, 1 * 118 }, { 256, 118 }));
        BFcol = (BFcol + 1) % 14;
    }
    if (Barracuda.getPosition().y == 690 || Barracuda.getPosition().y == 0)
    {
        BFvelocityY_AXIS *= -1;
    }
}

void QueenTriggerFish()
{
    QueenTrigger.move({ QTvelocityX_AXIS, 0 });
    QTrow = 0;
    if (QueenTrigger.getPosition().x == 0 || QueenTrigger.getPosition().x == 1460)
    {
        cu = 0;
        QTcol = 0;
        while (true)
        {
            QueenTrigger.setTextureRect(IntRect({ QTcol * 298, 2 * 216 }, { 298, 216 }));
            QTcol++;
            if (QTcol == 6)
            {
                QTcol = 0;
                QueenTrigger.setScale({ QTchangedir, 1 });
                QTvelocityX_AXIS = QTvelocityX_AXIS * -1;
                break;
            }
        }
        QTchangedir *= -1;
    }
    else if (QueenTriggercollieder.getGlobalBounds().findIntersection(smallfishs[num].sprite.getGlobalBounds()) || QueenTriggercollieder.getGlobalBounds().findIntersection(smallfishs[2].sprite.getGlobalBounds()))
    {
        if (cu < 1)
        {
            QTcol = 0;
        }
        if (cu == 3)
        {
            smallfishs[2].sprite.setPosition({ 3000, 3000 });
            smallfishs[0].sprite.setPosition({ 3000, 3000 });
        }
        cu++;
        if (cu <= 6)
        {
            QueenTrigger.setTextureRect(IntRect({ QTcol * 298, 0 }, { 298, 216 }));
            QTcol++;
        }
    }
    else
    {
        cu = 0;
        QueenTrigger.setTextureRect(IntRect({ QTcol * 298, 1 * 216 }, { 298, 216 }));
        QTcol = (QTcol + 1) % 14;
    }
}
void SwitchUser()
{
    StartSwitchUser();
    Clock frameClock;
    while (window.isOpen())
    {
        float dt = frameClock.restart().asSeconds();
        totaltime += dt;
        BarracudaFishanimation();
        QueenTriggerFish();
        for (auto& obj : smallfishs)
        {
            obj.shape.setPosition({ obj.sprite.getPosition().x, obj.sprite.getPosition().y });
            obj.update(286, 126);
            float posX = obj.sprite.getPosition().x;
            if (posX <= -150.f || posX >= WindowWidth + 150.f)
            {
                obj.velocityX_AXIS *= -1;
            }
        }
        UpdateSwitchUser();
        if (isUserSelected)
        {
            isUserSelected = 0;
            return;
        }
        DisplaySwitchUser();
    }
}

void ResetStats()
{
    NameEntry = false;
    InputString = "";
    SelectedUser = -1;
    isUserSelected = false;
    isListFull = false;
    DupplicateName = false;
    isConfirmUserDelete = false;
    CamefromDupplicate = false;
    JustOpenDeleteConfirm = false;
    isCancelAddingUser = false;
    NumberOfUsers = 0;
    DupplicateName = false;
    CamefromDupplicate = false;
    for (int i = 0; i < 8; i++)
    {
        delete PlayersTexts[i];
        PlayersTexts[i] = nullptr;
    }
}

void StartSwitchUser()
{
    ResetStats();
    float X = WindowWidth / 2.f, Y = WindowHeight / 2.f;
    CreateButton(Full, FullTex, "C:/Users/MOHAMED/source/repos/test1/x64/Debug/Assets/Switch User/BG.png", X, Y, 0.2, 0.2);
    X = WindowWidth * 0.25f, Y = WindowHeight * 0.78f;
    CreateButton(NewButton, NewButtonTex, "C:/Users/MOHAMED/source/repos/test1/x64/Debug/Assets/Switch User/Button.png", X, Y, 1.2, 1.2);
    if (!NewButtonHLTex.loadFromFile("C:/Users/MOHAMED/source/repos/test1/x64/Debug/Assets/Switch User/Button High.png"))
        cout << "Failed to load: ButtonHL" << "\n";
    X = WindowWidth * 0.5f, Y = WindowHeight * 0.78f;
    CreateButton(SelectButton, SelectButtonTex, "C:/Users/MOHAMED/source/repos/test1/x64/Debug/Assets/Switch User/Button.png", X, Y, 1.2, 1.2);
    if (!SelectButtonHLTex.loadFromFile("C:/Users/MOHAMED/source/repos/test1/x64/Debug/Assets/Switch User/Button High.png"))
        cout << "Failed to load: ButtonHL" << "\n";
    X = WindowWidth * 0.75f, Y = WindowHeight * 0.78f;
    CreateButton(DeleteButton, DeleteButtonTex, "C:/Users/MOHAMED/source/repos/test1/x64/Debug/Assets/Switch User/Button.png", X, Y, 1.2, 1.2);
    if (!DeleteButtonHLTex.loadFromFile("C:/Users/MOHAMED/source/repos/test1/x64/Debug/Assets/Switch User/Button High.png"))
        cout << "Failed to load: ButtonHL" << "\n";
    if (!btnFont.openFromFile("C:/Users/MOHAMED/source/repos/test1/x64/Debug/Assets/Fonts/BarmenoBold.otf"))
        cout << "Can't load font\n";
    SetupButtonText(NewText, "New", NewButton);
    SetupButtonText(SelectText, "Select", SelectButton);
    SetupButtonText(DeleteText, "Delete", DeleteButton);
    X = WindowWidth * 0.5f, Y = WindowHeight * 0.175f;
    CreateButton(Title, TitleTex, "C:/Users/MOHAMED/source/repos/test1/x64/Debug/Assets/Switch User/shell_chooseuser_hdr.png", X, Y, 1.5, 1.5);
    for (auto& player : players)
        player = { "", 0 };
    ifstream loadusers("C:/Users/MOHAMED/source/repos/test1/x64/Debug/Assets/Switch User/Users_List.txt");
    string name;
    int id;
    while (loadusers >> name >> id and NumberOfUsers < MaxNumberOfUsers)
    {
        players[NumberOfUsers].name = name;
        players[NumberOfUsers].id = id;
        NumberOfUsers++;
    }
    loadusers.close();
    RefreshUsersList();
    SelectUserHL.setSize({ WindowWidth * 0.35f, 45.f });
    SelectUserHL.setFillColor(Color(180, 220, 255, 150));
    SelectUserHL.setOrigin({ SelectUserHL.getSize().x / 2.f, 0.f });
}

void SetupButtonText(Text& text, const string& str, Sprite& button)
{
    text.setFont(btnFont);
    text.setString(str);
    text.setCharacterSize(30);
    text.setFillColor(Color(210, 240, 90));
    text.setOutlineColor(Color(30, 60, 10));
    text.setOutlineThickness(2.f);
    FloatRect bounds = text.getLocalBounds();
    text.setOrigin({ bounds.size.x / 2.f, bounds.size.y / 2.f });
    Vector2f center = { button.getPosition().x, button.getPosition().y - 10 };
    text.setPosition(center);
}

void CreateButton(Sprite& sprite, Texture& texture, const string& filePath,
    float xPosition, float yPositon, float scaleX, float scaleY)
{
    if (!texture.loadFromFile(filePath))
        cout << "Failed to load: " << filePath << "\n";
    sprite = Sprite(texture);
    sprite.setOrigin({ texture.getSize().x / 2.f, texture.getSize().y / 2.f });
    sprite.setPosition({ xPosition, yPositon });
    sprite.setScale({ scaleX, scaleY });
}

void UpdateSwitchUser()
{
    PlayingSound(false);
    Vector2f mousePos = window.mapPixelToCoords(Mouse::getPosition(window), view);
    while (auto event = window.pollEvent())
    {
        if (event->is<Event::Closed>())
            window.close();
        else if (const auto* resizeEvent = event->getIf<Event::Resized>())
        {
            view.setViewport(FloatRect({ 0.f, 0.f }, { 1.f, 1.f }));
            window.setView(view);
        }
        if (auto mouseEvent = event->getIf<Event::MouseButtonReleased>())
            if (mouseEvent->button == Mouse::Button::Left)
            {
                if (NewButton.getGlobalBounds().contains(mousePos) and !isConfirmUserDelete and !isListFull)
                    EnterYourName();
                if (NameEntry and DoneAddingUser.getGlobalBounds().contains(mousePos))
                {
                    if (NumberOfUsers < MaxNumberOfUsers)
                    {
                        if (InputString.empty())
                        {
                            NameEntry = 0;
                            isCancelAddingUser = 0;
                        }
                        else
                        {
                            NameEntry = 0;
                            isCancelAddingUser = 0;
                            for (int id = 0; id < NumberOfUsers; id++)
                            {
                                if (players[id].name == InputString)
                                {
                                    DupplicateName = 1;
                                    break;
                                }
                            }
                            if (DupplicateName)
                            {
                                if (!CamefromDupplicate)
                                    InputString = "";
                                DupplicateUser();
                            }
                            if (!DupplicateName)
                            {
                                players[NumberOfUsers].name = InputString;
                                players[NumberOfUsers].id = NumberOfUsers + 1;
                                NumberOfUsers++;
                                SaveUsers();
                                InputString = "";
                                RefreshUsersList();
                                CurUser = players[NumberOfUsers - 1].name;
                            }
                        }
                    }
                    else if (NumberOfUsers >= MaxNumberOfUsers)
                    {
                        NameEntry = 0;
                        FullList();
                        if (isListFull)
                            if (mouseEvent->button == Mouse::Button::Left)
                                if (FullOKButton.getGlobalBounds().contains(mousePos))
                                    isListFull = 0;
                    }
                }
                else if (NameEntry and CancelAddingUser.getGlobalBounds().contains(mousePos))
                {
                    InputString = "";
                    NameEntry = 0;
                    isCancelAddingUser = 1;
                    DupplicateName = 0;
                    CamefromDupplicate = 0;
                }
                for (int i = 0; i < NumberOfUsers; i++)
                {
                    if (PlayersTexts[i] != nullptr and PlayersTexts[i]->getGlobalBounds().contains(mousePos))
                    {
                        SelectedUser = i;
                        float X = WindowWidth * 0.5f - 5, Y = PlayersTexts[i]->getPosition().y - 5.f;
                        SelectUserHL.setPosition({ X, Y });
                    }
                }
                if (SelectButton.getGlobalBounds().contains(mousePos) and !NameEntry and !isCancelAddingUser and !isListFull and !DupplicateName and SelectedUser >= 0 and SelectedUser < NumberOfUsers)
                {
                    CurUser = players[SelectedUser].name;
                    isUserSelected = 1;
                }
                if (SelectedUser != -1 and !NameEntry and !isConfirmUserDelete and DeleteButton.getGlobalBounds().contains(mousePos))
                    DeleteUser();
                if (isConfirmUserDelete)
                {
                    if (!JustOpenDeleteConfirm and NoButton.getGlobalBounds().contains(mousePos))
                        isConfirmUserDelete = 0;
                    if (YesButton.getGlobalBounds().contains(mousePos))
                    {
                        for (int id = SelectedUser; id < NumberOfUsers - 1; id++)
                        {
                            players[id].name = players[id + 1].name;
                            players[id].id = id + 1;
                        }
                        players[NumberOfUsers - 1].name = "";
                        players[NumberOfUsers - 1].id = 0;
                        NumberOfUsers--;
                        SaveUsers();
                        isConfirmUserDelete = 0;
                        SelectedUser = -1;
                        RefreshUsersList();
                    }
                    JustOpenDeleteConfirm = 0;
                }
                if (FullOKButton.getGlobalBounds().contains(mousePos))
                    if (isListFull and !NameEntry and !isConfirmUserDelete and !DupplicateName)
                        isListFull = 0;
                if (DupplicateOKButton.getGlobalBounds().contains(mousePos))
                {
                    if (DupplicateName and !isListFull and !isConfirmUserDelete and !NameEntry and !isUserSelected)
                    {
                        DupplicateName = 0;
                        CamefromDupplicate = 1;
                        NameEntry = 1;
                    }
                }
            }
        if (const auto* typed = event->getIf<Event::TextEntered>())
        {
            char32_t a = typed->unicode;
            if (a == 8 and !InputString.empty())
                InputString.pop_back();
            else if (a >= 32 and InputString.size() < 8)
                InputString += a;
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
        float X = DisplayText.getPosition().x + textBounds.size.x + 5, Y = DisplayText.getPosition().y + 25;
        Blink.setPosition({ X, Y });
        DisplayText.setString(InputString);
    }
    HoverButton(NewButton, NewButtonTex, NewButtonHLTex, mousePos, NewText);
    HoverButton(SelectButton, NewButtonTex, NewButtonHLTex, mousePos, SelectText);
    HoverButton(DeleteButton, NewButtonTex, NewButtonHLTex, mousePos, DeleteText);
    if (NameEntry and !isConfirmUserDelete)
    {
        HoverButton(DoneAddingUser, DoneAddingUserTex, DoneAddingUserHLTex, mousePos, DoneAddingUserText);
        HoverButton(CancelAddingUser, CancelAddingUserTex, CancelAddingUserHLTex, mousePos, CancelAddingUserText);
    }
    if (isConfirmUserDelete and !NameEntry)
    {
        HoverButton(YesButton, NewButtonTex, NewButtonHLTex, mousePos, YesButtonText);
        HoverButton(NoButton, NewButtonTex, NewButtonHLTex, mousePos, NoButtonText);
    }
    if (!NameEntry and !isConfirmUserDelete and isListFull and !DupplicateName)
        HoverButton(FullOKButton, NewButtonTex, NewButtonHLTex, mousePos, OKText);
    if (!NameEntry and !isConfirmUserDelete and !isListFull and DupplicateName)
        HoverButton(DupplicateOKButton, NewButtonTex, NewButtonHLTex, mousePos, DupplicatedUserText);
}

void HoverButton(Sprite& button, const Texture& normalTex, const Texture& highlightTex, Vector2f mousePos, Text& text)
{
    if (button.getGlobalBounds().contains(mousePos))
    {
        button.setTexture(highlightTex);
        text.setFillColor(Color(255, 255, 80));
    }
    else
    {
        button.setTexture(normalTex);
        text.setFillColor(Color(210, 240, 90));
    }
}

void EnterYourName()
{
    if (NameEntry)
        return;
    if (NumberOfUsers >= MaxNumberOfUsers)
    {
        FullList();
        return;
    }
    DupplicateName = 0;
    CamefromDupplicate = 0;
    isUserSelected = 0;
    Vector2f mousePos = static_cast<Vector2f>(Mouse::getPosition(window));
    NameEntry = 1;
    unsigned int X = WindowWidth * 0.5f, Y = WindowHeight * 0.5f;
    CreateButton(EnterYourNamebg, EnterYourNameBgTex, "C:/Users/MOHAMED/source/repos/test1/x64/Debug/Assets/Switch User/EnterYourName.png", X, Y, 0.2, 0.2);
    if (!Inputfont.openFromFile("C:/Users/MOHAMED/source/repos/test1/x64/Debug/Assets/Fonts/trebuc.ttf"))
        cout << "Cant Open Font!";
    DisplayText.setFont(Inputfont);
    DisplayText.setCharacterSize(30);
    DisplayText.setFillColor(Color::White);
    X = WindowWidth * 0.24f, Y = WindowHeight * 0.34f;
    DisplayText.setPosition({ (float)X, (float)Y });
    X = WindowWidth * 0.375f, Y = WindowHeight * 0.448f;
    CreateButton(Blink, BlinkTex, "C:/Users/MOHAMED/source/repos/test1/x64/Debug/Assets/Switch User/shell_editboxcursor.jpg", X, Y, 1, 1);
    X = WindowWidth * 0.5f, Y = WindowHeight * 0.62f;
    CreateButton(DoneAddingUser, DoneAddingUserTex, "C:/Users/MOHAMED/source/repos/test1/x64/Debug/Assets/Switch User/Button.png", X, Y, 1.25, 1.25);
    SetupButtonText(DoneAddingUserText, "Done", DoneAddingUser);
    if (!DoneAddingUserHLTex.loadFromFile("C:/Users/MOHAMED/source/repos/test1/x64/Debug/Assets/Switch User/Button High.png"))
        cout << "Failed to load: ButtonHL" << "\n";
    if (!CancelAddingUserHLTex.loadFromFile("C:/Users/MOHAMED/source/repos/test1/x64/Debug/Assets/Switch User/shell_tinybtn124_high.jpg"))
        cout << "Can't load Texture!\n";
    X = WindowWidth * 0.5f, Y = WindowHeight * 0.75f;
    CreateButton(CancelAddingUser, CancelAddingUserTex, "C:/Users/MOHAMED/source/repos/test1/x64/Debug/Assets/Switch User/Cancel.png", X, Y, 1.5, 1.5);
    SetupButtonText(CancelAddingUserText, "Cancel", CancelAddingUser);
    CancelAddingUserText.setPosition({ CancelAddingUser.getPosition().x, CancelAddingUser.getPosition().y - 5 });
}

void RefreshUsersList()
{
    for (int id = 0; id < 8; id++)
    {
        delete PlayersTexts[id];
        PlayersTexts[id] = nullptr;
    }
    for (int id = 0; id < NumberOfUsers; id++)
    {
        PlayersTexts[id] = new Text(btnFont, players[id].name, 25);
        PlayersTexts[id]->setFillColor(Color::White);
        float X = WindowWidth * 0.5f;
        float Y = WindowHeight * 0.23f + (id * 40.f);
        FloatRect bounds = PlayersTexts[id]->getLocalBounds();
        PlayersTexts[id]->setOrigin({ bounds.size.x / 2.f, 0.f });
        PlayersTexts[id]->setPosition({ X, Y });
    }
}

void DeleteUser()
{
    JustOpenDeleteConfirm = 1;
    isConfirmUserDelete = 1;
    unsigned int X = WindowWidth * 0.5f, Y = WindowHeight * 0.5f;
    CreateButton(DeleteUserBg, DelteUserBgTex, "C:/Users/MOHAMED/source/repos/test1/x64/Debug/Assets/Switch User/DeleteUserBg.png", X, Y, 0.175, 0.175);
    X = WindowWidth * 0.4f, Y = WindowHeight * 0.7f;
    CreateButton(YesButton, NewButtonTex, "C:/Users/MOHAMED/source/repos/test1/x64/Debug/Assets/Switch User/Button.png", X, Y, 1.25, 1.25);
    SetupButtonText(YesButtonText, "Yes", YesButton);
    X = WindowWidth * 0.6f, Y = WindowHeight * 0.7f;
    CreateButton(NoButton, NewButtonTex, "C:/Users/MOHAMED/source/repos/test1/x64/Debug/Assets/Switch User/Button.png", X, Y, 1.25, 1.25);
    SetupButtonText(NoButtonText, "No!", NoButton);
    SetupButtonText(DeletethisUser, players[SelectedUser].name, DeleteUserBg);
    X = WindowWidth * 0.47f, Y = WindowHeight * 0.52f;
    DeletethisUser.setPosition({ (float)X, (float)Y });
    DeletethisUser.setCharacterSize(45);
}

void FullList()
{
    isListFull = 1;
    unsigned int X = WindowWidth * 0.5f, Y = WindowHeight * 0.5f;
    CreateButton(ListisFull, ListisFullTex, "C:/Users/MOHAMED/source/repos/test1/x64/Debug/Assets/Switch User/ListIsFull.png", X, Y, 0.2, 0.2);
    X = WindowWidth * 0.5f, Y = WindowHeight * 0.64f;
    CreateButton(FullOKButton, FullOKButtonTex, "C:/Users/MOHAMED/source/repos/test1/x64/Debug/Assets/Switch User/Button.png", X, Y, 1.5, 1.5);
    SetupButtonText(FullListOKBtnText, "OK", FullOKButton);
    InputString = "";
}

void DupplicateUser()
{
    DupplicateName = 1;
    unsigned int X = WindowWidth * 0.5f, Y = WindowHeight * 0.5f;
    CreateButton(DupplicateBg, DupplicateBgTex, "C:/Users/MOHAMED/source/repos/test1/x64/Debug/Assets/Switch User/DupplicateUser.png", X, Y, 0.2, 0.2);
    X = WindowWidth * 0.5f, Y = WindowHeight * 0.72f;
    CreateButton(DupplicateOKButton, DupplicateOKButtonTex, "C:/Users/MOHAMED/source/repos/test1/x64/Debug/Assets/Switch User/Button.png", X, Y, 1.5, 1.5);
    SetupButtonText(DupplicateOKBtnText, "OK", DupplicateOKButton);
    SetupButtonText(DupplicatedUserText, InputString, DupplicateBg);
    X = WindowWidth * 0.48f, Y = WindowHeight * 0.56f;
    DupplicatedUserText.setPosition({ (float)X, (float)Y });
    DupplicatedUserText.setCharacterSize(40);
}

void DisplaySwitchUser()
{
    DrawMainMenuBackground();
    window.draw(Full);
    window.draw(Title);
    window.draw(NewButton);
    window.draw(SelectButton);
    window.draw(DeleteButton);
    window.draw(NewText);
    window.draw(SelectText);
    window.draw(DeleteText);
    for (int user = 0; user < NumberOfUsers; user++)
        if (PlayersTexts[user] != nullptr)
            window.draw(*PlayersTexts[user]);
    if (SelectedUser >= 0)
        window.draw(SelectUserHL);
    if (isUserSelected)
    {
        isUserSelected = 0;
        return;
    }
    if (NameEntry and !isConfirmUserDelete and !isListFull and !DupplicateName)
    {
        DrawMainMenuBackground();
        window.draw(EnterYourNamebg);
        window.draw(DisplayText);
        if (drawtheCursor)
            window.draw(Blink);
        window.draw(DoneAddingUser);
        window.draw(DoneAddingUserText);
        window.draw(CancelAddingUser);
        window.draw(CancelAddingUserText);
    }
    if (isConfirmUserDelete and SelectedUser != -1 and !NameEntry and !isListFull and !DupplicateName)
    {
        DrawMainMenuBackground();
        window.draw(DeleteUserBg);
        window.draw(YesButton);
        window.draw(NoButton);
        window.draw(YesButtonText);
        window.draw(NoButtonText);
        window.draw(DeletethisUser);
    }
    if (isListFull and !isConfirmUserDelete and !NameEntry and !DupplicateName and !isUserSelected)
    {
        DrawMainMenuBackground();
        window.draw(ListisFull);
        window.draw(FullOKButton);
        window.draw(FullListOKBtnText);
    }
    if (DupplicateName and !isListFull and !isConfirmUserDelete and !NameEntry and !isUserSelected)
    {
        DrawMainMenuBackground();
        window.draw(DupplicateBg);
        window.draw(DupplicateOKButton);
        window.draw(DupplicateOKBtnText);
        window.draw(DupplicatedUserText);
    }
    window.display();
}

void OptionsMenu()
{
    shouldCloseOptions = false;
    StartOptions();
    Clock clock;
    while (window.isOpen())
    {
        float dt1 = clock.restart().asSeconds();
        totaltime += dt1;
        BarracudaFishanimation();
        QueenTriggerFish();
        for (auto& obj : smallfishs)
        {
            obj.shape.setPosition({ obj.sprite.getPosition().x, obj.sprite.getPosition().y });
            obj.update(286, 126);
            float posX = obj.sprite.getPosition().x;
            if (posX <= -150.f || posX >= WindowWidth + 150.f)
            {
                obj.velocityX_AXIS *= -1;
            }
        }
        float dt = clock.restart().asSeconds();
        while (const optional event = window.pollEvent())
        {
            if (event->is<Event::Closed>())
                window.close();
            else if (const auto* resizeEvent = event->getIf<Event::Resized>())
            {
                view.setViewport(FloatRect({ 0.f, 0.f }, { 1.f, 1.f }));
                window.setView(view);
            }
        }
        UpdateOptions();
        if (shouldCloseOptions)
            return;
        DrawOptions();
    }
}

void setRightAligned(optional<Text>& txt, float x, float y)
{
    if (txt)
    {
        FloatRect bounds = txt->getLocalBounds();
        txt->setOrigin({ bounds.size.x, bounds.size.y / 2.f });
        txt->setPosition({ x, y });
    }
}

void setTopCentered(optional<Text>& txt, float x, float y)
{
    if (txt)
    {
        FloatRect bounds = txt->getLocalBounds();
        txt->setOrigin({ bounds.size.x / 2.f, 0.f });
        txt->setPosition({ x, y });
    }
}

void drawTextWithShadow(RenderWindow& win, optional<Text>& textObj)
{
    if (textObj)
    {
        Text shadow = *textObj;
        shadow.setFillColor(Color(10, 20, 40, 180));
        shadow.setOutlineThickness(0.f);
        shadow.move({ 1.f, 1.f });
        win.draw(shadow);
        win.draw(*textObj);
    }
}

void drawCenteredSprite(sf::RenderWindow& window, sf::Sprite& sprite, sf::Texture& texture, int opacity)
{
    sprite.setTexture(texture);
    sprite.setColor(sf::Color(255, 255, 255, opacity));
    auto textureSize = sf::Vector2f(texture.getSize());
    sprite.setOrigin(textureSize / 2.f);
    window.draw(sprite);
}

void StartOptions()
{
    window.setFramerateLimit(60);
    window.setView(view);
    srand(static_cast<unsigned int>(time(NULL)));
    if (!globalFont.openFromFile("C:/Users/MOHAMED/source/repos/test1/x64/Debug/Assets/Fonts/Barmeno.ttf"))
        cout << "Failed to load font!" << endl;
    globalFont.setSmooth(false);
    screenDarkener.setSize(Vector2f(WindowWidth, WindowHeight));
    screenDarkener.setFillColor(Color(0, 0, 0, 95));
    if (texBgPlank.loadFromFile("C:/Users/MOHAMED/source/repos/test1/x64/Debug/Assets/Options/plank.png"))
    {
        texBgPlank.setSmooth(true);
        sprBgPlank.emplace(texBgPlank);
        FloatRect b = sprBgPlank->getLocalBounds();
        sprBgPlank->setOrigin({ b.size.x / 2.f, b.size.y / 2.f });
        sprBgPlank->setPosition({ WindowWidth / 2.f, 280.f });
    }
    if (texCorals.loadFromFile("C:/Users/MOHAMED/source/repos/test1/x64/Debug/Assets/Options/corals.png"))
    {
        texCorals.setSmooth(true);
        sprCorals.emplace(texCorals);
        FloatRect b = sprCorals->getLocalBounds();
        sprCorals->setOrigin({ b.size.x / 2.f, b.size.y / 2.f });
        sprCorals->setPosition({ (WindowWidth - 2) / 2.f, 315.f });
        sprCorals->setColor(Color(255, 255, 255, 175));
    }
    if (texTitle.loadFromFile("C:/Users/MOHAMED/source/repos/test1/x64/Debug/Assets/Options/options_title.png"))
    {
        texTitle.setSmooth(true);
        sprTitle.emplace(texTitle);
        FloatRect b = sprTitle->getLocalBounds();
        sprTitle->setOrigin({ b.size.x / 2.f, b.size.y / 2.f });
        sprTitle->setPosition({ WindowWidth / 2.f, 100.f });
        sprTitle->setScale({ 1.f, 1.f });
    }
    if (WaterSound.loadFromFile("C:/Users/MOHAMED/source/repos/test1/x64/Debug/Assets/Music and Sounds/waterloop1.ogg"))
    {
        waterloop.emplace(WaterSound);
        waterloop->setLooping(true);
    }
    (void)sfxBuffers[0].loadFromFile("C:/Users/MOHAMED/source/repos/test1/x64/Debug/Assets/Music and Sounds/wateramb1.ogg");
    (void)sfxBuffers[1].loadFromFile("C:/Users/MOHAMED/source/repos/test1/x64/Debug/Assets/Music and Sounds/wateramb2.ogg");
    (void)sfxBuffers[2].loadFromFile("C:/Users/MOHAMED/source/repos/test1/x64/Debug/Assets/Music and Sounds/wave1.ogg");
    (void)sfxBuffers[3].loadFromFile("C:/Users/MOHAMED/source/repos/test1/x64/Debug/Assets/Music and Sounds/wave2.ogg");
    nextSfxDelay = (rand() % 2500 + 2500) / 1000.f;
    (void)texUncheckedNormal.loadFromFile("C:/Users/MOHAMED/source/repos/test1/x64/Debug/Assets/Options/_shell_checkbtn_normal.png");
    (void)texUncheckedHover.loadFromFile("C:/Users/MOHAMED/source/repos/test1/x64/Debug/Assets/Options/_shell_checkbtn_high.png");
    (void)texCheckedNormal.loadFromFile("C:/Users/MOHAMED/source/repos/test1/x64/Debug/Assets/Options/shell_checkbtnchecked_normal.png");
    (void)texCheckedHover.loadFromFile("C:/Users/MOHAMED/source/repos/test1/x64/Debug/Assets/Options/_shell_checkbtnchecked_high.png");
    (void)texDoneNormal.loadFromFile("C:/Users/MOHAMED/source/repos/test1/x64/Debug/Assets/Options/done_normal.png");
    (void)texDoneHover.loadFromFile("C:/Users/MOHAMED/source/repos/test1/x64/Debug/Assets/Options/done_hover.png");
    texUncheckedNormal.setSmooth(true);
    texUncheckedHover.setSmooth(true);
    texCheckedNormal.setSmooth(true);
    texCheckedHover.setSmooth(true);
    texDoneNormal.setSmooth(true);
    texDoneHover.setSmooth(true);
    Color paleText(225, 235, 245);
    mouseLabel.emplace(globalFont);
    mouseLabel->setString("Mouse Speed:");
    mouseLabel->setCharacterSize(22);
    mouseLabel->setFillColor(paleText);
    setRightAligned(mouseLabel, LAYOUT_LABEL_X, POS_Y_MOUSE_SPEED);
    detailLabel.emplace(globalFont);
    detailLabel->setString("Game Detail:");
    detailLabel->setCharacterSize(22);
    detailLabel->setFillColor(paleText);
    setRightAligned(detailLabel, LAYOUT_LABEL_X, POS_Y_GAME_DETAIL);
    versionLabel.emplace(globalFont);
    versionLabel->setString("Game Version: 1.0.0");
    versionLabel->setCharacterSize(16);
    versionLabel->setPosition({ 170.f, 423.f });
    versionLabel->setFillColor(Color(200, 210, 220));
    const char* labels[] = { "Sound:", "Music:", "Full Screen:", "Slow", "", "", "Fast", "Low", "", "High", "Done" };
    for (int i = 0; i < NumOptions; i++)
    {
        OptionButtons[i].text.emplace(globalFont);
        OptionButtons[i].text->setString(labels[i]);
        OptionButtons[i].isCheckbox = true;
        OptionButtons[i].isHovered = false;
        OptionButtons[i].checkbox.emplace(texUncheckedNormal);
        if (i < 3)
        {
            if (i == 2)
                OptionButtons[i].isChecked = true;
            else
                OptionButtons[i].isChecked = true;
            OptionButtons[i].text->setCharacterSize(22);
            OptionButtons[i].text->setFillColor(paleText);
            float yPos = POS_Y_TOP_OPTIONS + (i * LAYOUT_ROW_SPACING);
            setRightAligned(OptionButtons[i].text, LAYOUT_LABEL_X, yPos);
            OptionButtons[i].checkbox->setPosition({ LAYOUT_BUBBLE_START_X, yPos + 7.f });
        }
        else if (i >= 3 && i <= 6)
        {
            OptionButtons[i].isChecked = (i == 3);
            OptionButtons[i].text->setCharacterSize(14);
            OptionButtons[i].text->setFillColor(paleText);
            float startX = LAYOUT_BUBBLE_START_X + ((i - 3) * LAYOUT_COL_SPACING);
            OptionButtons[i].checkbox->setPosition({ startX, POS_Y_MOUSE_SPEED + 7.f });
            setTopCentered(OptionButtons[i].text, startX, POS_Y_MOUSE_SPEED + 20.f);
        }
        else if (i >= 7 && i <= 9)
        {
            OptionButtons[i].isChecked = (i == 9);
            OptionButtons[i].text->setCharacterSize(14);
            OptionButtons[i].text->setFillColor(paleText);
            float startX = LAYOUT_BUBBLE_START_X + ((i - 7) * LAYOUT_COL_SPACING);
            OptionButtons[i].checkbox->setPosition({ startX, POS_Y_GAME_DETAIL + 7.f });
            setTopCentered(OptionButtons[i].text, startX, POS_Y_GAME_DETAIL + 20.f);
        }
        else
        {
            OptionButtons[i].isCheckbox = false;
            OptionButtons[i].checkbox->setTexture(texDoneNormal, true);
            FloatRect plankBounds = OptionButtons[i].checkbox->getLocalBounds();
            OptionButtons[i].checkbox->setOrigin({ plankBounds.size.x / 2.f, plankBounds.size.y / 2.f });
            OptionButtons[i].checkbox->setPosition({ WindowWidth / 2.f, 450.f });
            OptionButtons[i].text->setCharacterSize(26);
            OptionButtons[i].text->setFillColor(Color(180, 255, 50));
            OptionButtons[i].text->setOutlineColor(Color(80, 40, 0));
            OptionButtons[i].text->setOutlineThickness(3.f);
            FloatRect textBounds = OptionButtons[i].text->getLocalBounds();
            OptionButtons[i].text->setOrigin({ textBounds.size.x / 2.f, textBounds.size.y / 2.f });
            OptionButtons[i].text->setPosition({ WindowWidth / 2.f, 445.f });
        }
    }
}

void UpdateOptions()
{
    PlayingSound(false);
    auto MousePosition = window.mapPixelToCoords(Mouse::getPosition(window), view);
    bool currentMousePressed = Mouse::isButtonPressed(Mouse::Button::Left);
    for (int i = 0; i < NumOptions; i++)
    {
        bool overText = OptionButtons[i].text && OptionButtons[i].text->getGlobalBounds().contains(MousePosition);
        bool overBubble = false;
        bool overDoneButton = false;
        if (OptionButtons[i].isCheckbox && OptionButtons[i].checkbox)
        {
            Vector2f pos = OptionButtons[i].checkbox->getPosition();
            if (abs(MousePosition.x - pos.x) < 16.f && abs(MousePosition.y - pos.y) < 16.f)
                overBubble = true;
        }
        else if (!OptionButtons[i].isCheckbox && i == 10 && OptionButtons[i].checkbox)
        {
            if (OptionButtons[i].checkbox->getGlobalBounds().contains(MousePosition))
                overDoneButton = true;
        }
        if (overText || overBubble || overDoneButton)
        {
            OptionButtons[i].isHovered = true;
            if (currentMousePressed && !prevMousePressed)
            {
                if (i == 10)
                {
                    shouldCloseOptions = true;
                    return;
                }
                else if (i >= 3 && i <= 6)
                {
                    for (int j = 3; j <= 6; j++)
                        OptionButtons[j].isChecked = false;
                    OptionButtons[i].isChecked = true;
                    MSpeedIndex = (i - 3) + 1;
                }
                else if (i >= 7 && i <= 9)
                {
                    for (int j = 7; j <= 9; j++)
                        OptionButtons[j].isChecked = false;
                    OptionButtons[i].isChecked = true;
                    GraphicsIndex = i - 7;
                }
                else if (i == 2)
                {
                    OptionButtons[i].isChecked = !OptionButtons[i].isChecked;
                    if (OptionButtons[i].isChecked)
                        window.create(VideoMode::getDesktopMode(), "Feeding Frenzy 2", State::Fullscreen);
                    else
                        window.create(VideoMode({ 800, 600 }), "Feeding Frenzy 2", State::Windowed);
                    view.setSize({ 800.f, 600.f });
                    view.setCenter({ 400.f, 300.f });
                    view.setViewport(FloatRect({ 0.f, 0.f }, { 1.f, 1.f }));
                    window.setView(view);
                    window.setFramerateLimit(60);
                }
                else
                {
                    OptionButtons[i].isChecked = !OptionButtons[i].isChecked;
                }
            }
        }
        else
        {
            OptionButtons[i].isHovered = false;
        }
        if (!OptionButtons[i].isCheckbox && OptionButtons[i].text)
        {
            if (OptionButtons[i].isHovered)
                OptionButtons[i].text->setFillColor(Color(240, 210, 0));
            else
                OptionButtons[i].text->setFillColor(Color(180, 255, 50));
        }
    }
    if (OptionButtons[0].isChecked)
    {
        if (waterloop && waterloop->getStatus() != Sound::Status::Playing)
            waterloop->play();
    }
    else
    {
        if (waterloop)
            waterloop->stop();
        for (int c = 0; c < NUM_SOUND_CHANNELS; c++)
            if (sfxChannels[c])
                sfxChannels[c]->stop();
    }
    isMusicEnabled = OptionButtons[1].isChecked;
    if (isMusicEnabled)
    {
        if (mainmenumusic.getStatus() != Music::Status::Playing)
            mainmenumusic.play();
    }
    else
    {
        if (mainmenumusic.getStatus() == Music::Status::Playing)
            mainmenumusic.pause();
    }
    prevMousePressed = currentMousePressed;
}

void DrawOptions()
{
    window.setView(view);
    DrawMainMenuBackground();
    window.draw(screenDarkener);
    if (sprBgPlank)
    {
        Sprite plankOutline = *sprBgPlank;
        plankOutline.setColor(Color(0, 0, 0, 90));
        Vector2f pos = sprBgPlank->getPosition();
        float outlineThickness = 3.f;
        plankOutline.setPosition({ pos.x - outlineThickness, pos.y });
        window.draw(plankOutline);
        plankOutline.setPosition({ pos.x + outlineThickness, pos.y });
        window.draw(plankOutline);
        plankOutline.setPosition({ pos.x, pos.y - outlineThickness });
        window.draw(plankOutline);
        plankOutline.setPosition({ pos.x, pos.y + outlineThickness });
        window.draw(plankOutline);
        sprBgPlank->setPosition(pos);
        window.draw(*sprBgPlank);
    }
    if (sprCorals)
        window.draw(*sprCorals);
    if (sprTitle)
        window.draw(*sprTitle);
    drawTextWithShadow(window, mouseLabel);
    drawTextWithShadow(window, detailLabel);
    drawTextWithShadow(window, versionLabel);
    for (int i = 0; i < NumOptions; i++)
    {
        if (i >= 3 && i <= 9)
        {
            if (OptionButtons[i].text)
                drawTextWithShadow(window, OptionButtons[i].text);
        }
        else if (i == 10)
        {
            OptionButtons[i].checkbox->setScale({ 1.0f, 1.0f });
            drawCenteredSprite(window, *OptionButtons[i].checkbox, texDoneNormal, 230);
            if (OptionButtons[i].isHovered)
            {
                OptionButtons[i].checkbox->setScale({ 1.05f, 1.05f });
                drawCenteredSprite(window, *OptionButtons[i].checkbox, texDoneHover, 220);
            }
            drawTextWithShadow(window, OptionButtons[i].text);
            continue;
        }
        else
        {
            drawTextWithShadow(window, OptionButtons[i].text);
        }
        if (OptionButtons[i].isCheckbox && OptionButtons[i].checkbox)
        {
            if (OptionButtons[i].isHovered && OptionButtons[i].isChecked)
                OptionButtons[i].checkbox->setScale({ 1.1f, 1.1f });
            else
                OptionButtons[i].checkbox->setScale({ 1.0f, 1.0f });
            int stateOpacity = OptionButtons[i].isHovered ? 255 : 210;
            if (OptionButtons[i].isHovered)
            {
                drawCenteredSprite(window, *OptionButtons[i].checkbox, texUncheckedHover, 150);
            }
            if (OptionButtons[i].isChecked)
                drawCenteredSprite(window, *OptionButtons[i].checkbox, texCheckedNormal, stateOpacity);
            else
                drawCenteredSprite(window, *OptionButtons[i].checkbox, texUncheckedNormal, stateOpacity);
        }
    }
    window.display();
}

void UpdateMainMenuFish()
{
    GreenfishAnimation();
    MinowFishanimation();
    BarracudaFishanimation();
    QueenTriggerFish();
    for (auto& obj : smallfishs)
    {
        obj.shape.setPosition({ obj.sprite.getPosition().x, obj.sprite.getPosition().y });
        obj.update(286, 126);
        if (obj.sprite.getPosition().x <= 0 || obj.sprite.getPosition().x >= WindowWidth)
        {
            obj.velocityX_AXIS *= -1;
        }
    }
}

void QuitGame()
{
    Clock clock;
    StartQuit();
    while (window.isOpen())
    {
        float dt = clock.restart().asSeconds();
        totaltime += dt;
        BarracudaFishanimation();
        QueenTriggerFish();
        for (auto& obj : smallfishs)
        {
            obj.shape.setPosition({ obj.sprite.getPosition().x, obj.sprite.getPosition().y });
            obj.update(286, 126);
            float posX = obj.sprite.getPosition().x;
            if (posX <= -150.f || posX >= WindowWidth + 150.f)
            {
                obj.velocityX_AXIS *= -1;
            }
        }
        while (const optional event = window.pollEvent())
        {
            if (event->is<Event::Closed>())
            {
                window.close();
                return;
            }
            if (auto mouseEvent = event->getIf<Event::MouseButtonReleased>())
            {
                if (mouseEvent->button == Mouse::Button::Left)
                {
                    Vector2f mousePos = window.mapPixelToCoords(Mouse::getPosition(window), view);
                    Vector2f yesPos = { WindowWidth / 2.f - 100.f, WindowHeight / 2.f + 90.f };
                    Vector2f noPos = { WindowWidth / 2.f + 100.f, WindowHeight / 2.f + 90.f };
                    quit_button_sprite.setPosition(yesPos);
                    if (quit_button_sprite.getGlobalBounds().contains(mousePos))
                    {
                        window.close();
                        return;
                    }
                    quit_button_sprite.setPosition(noPos);
                    if (quit_button_sprite.getGlobalBounds().contains(mousePos))
                    {
                        return;
                    }
                }
            }
        }
        window.setView(view);
        UpdateQuit();
        DrawQuit();
    }
}

void StartQuit()
{
    if (!btnFont.openFromFile("C:/Users/MOHAMED/source/repos/test1/x64/Debug/Assets/Fonts/BarmenoBold.otf"))
    {
        cout << "Error: Could not load font!" << endl;
    }
    if (!quit_bg.loadFromFile("C:/Users/MOHAMED/source/repos/test1/x64/Debug/Assets/Quit/bg_quit.png"))
        cout << "Error Loading Quit BG!" << endl;
    quit_bg_sprite.setTexture(quit_bg);
    quit_bg.setSmooth(true);
    float targetWidth = 500.f;
    float targetHeight = 300.f;
    Vector2u texture_Size = quit_bg.getSize();
    float ScaleX = targetWidth / texture_Size.x;
    float ScaleY = targetHeight / texture_Size.y;
    quit_bg_sprite.setOrigin({ texture_Size.x / 2.f, texture_Size.y / 2.f });
    quit_bg_sprite.setPosition({ WindowWidth / 2.f, WindowHeight / 2.f });
    quit_bg_sprite.setScale({ ScaleX, ScaleY });
    quit_icon_sprite.setOrigin({ quit_icon.getSize().x / 2.f, quit_icon.getSize().y / 2.f });
    quit_icon_sprite.setPosition({ WindowWidth / 2.f, WindowHeight / 2.f - 133.f });
    quit_icon_sprite.setScale({ 0.25f, 0.25f });
    quit_button.setSmooth(true);
    quit_button_hover.setSmooth(true);
    quit_button_sprite.setTexture(quit_button);
    quit_button_hover_sprite.setTexture(quit_button_hover);
    Vector2f btnSize = { (float)quit_button.getSize().x, (float)quit_button.getSize().y };
    quit_button_sprite.setOrigin({ btnSize.x / 2.f, btnSize.y / 2.f });
    quit_button_hover_sprite.setOrigin({ btnSize.x / 2.f, btnSize.y / 2.f });
    quit_title_text.setFont(btnFont);
    quit_title_text.setString("Are you sure you want to quit");
    quit_title_text.setCharacterSize(28);
    quit_title_text.setFillColor(Color(255, 255, 255));
    quit_title_text.setOutlineThickness(2);
    quit_title_text.setOutlineColor(Color(80, 40, 0));
    FloatRect titleBounds = quit_title_text.getLocalBounds();
    quit_title_text.setOrigin({ titleBounds.position.x + titleBounds.size.x / 2.f, titleBounds.position.y + titleBounds.size.y / 2.f });
    quit_title_text.setPosition({ WindowWidth / 2.f, WindowHeight / 2.f - 30.f });
    quit_title_text2.setFont(btnFont);
    quit_title_text2.setString("Feeding Frenzy 2?");
    quit_title_text2.setCharacterSize(28);
    quit_title_text2.setFillColor(Color(255, 255, 255));
    quit_title_text2.setOutlineThickness(2);
    quit_title_text2.setOutlineColor(Color(80, 40, 0));
    FloatRect titleBounds2 = quit_title_text2.getLocalBounds();
    quit_title_text2.setOrigin({ titleBounds2.position.x + titleBounds2.size.x / 2.f, titleBounds2.position.y + titleBounds2.size.y / 2.f });
    quit_title_text2.setPosition({ WindowWidth / 2.f, WindowHeight / 2.f + 10.f });
    quit_yes_text.setFont(btnFont);
    quit_yes_text.setString("Yes");
    quit_yes_text.setCharacterSize(24);
    quit_yes_text.setFillColor(Color(180, 255, 100));
    quit_yes_text.setOutlineThickness(2);
    quit_yes_text.setOutlineColor(Color(20, 60, 0));
    FloatRect yesBounds = quit_yes_text.getLocalBounds();
    quit_yes_text.setOrigin({ yesBounds.position.x + yesBounds.size.x / 2.f, yesBounds.position.y + yesBounds.size.y / 2.f });
    quit_yes_text.setPosition({ WindowWidth / 2.f - 100.f, WindowHeight / 2.f + 90.f });
    quit_no_text.setFont(btnFont);
    quit_no_text.setString("No!");
    quit_no_text.setCharacterSize(24);
    quit_no_text.setFillColor(Color(180, 255, 100));
    quit_no_text.setOutlineThickness(2);
    quit_no_text.setOutlineColor(Color(20, 60, 0));
    FloatRect noBounds = quit_no_text.getLocalBounds();
    quit_no_text.setOrigin({ noBounds.position.x + noBounds.size.x / 2.f, noBounds.position.y + noBounds.size.y / 2.f });
    quit_no_text.setPosition({ WindowWidth / 2.f + 100.f, WindowHeight / 2.f + 90.f });
    quit_yes_pressed = false;
    quit_no_pressed = false;
}

void UpdateQuit()
{
    PlayingSound(false);
    Vector2f mousePos = window.mapPixelToCoords(Mouse::getPosition(window), view);
    Vector2f yesPos = { WindowWidth / 2.f - 100.f, WindowHeight / 2.f + 90.f };
    Vector2f noPos = { WindowWidth / 2.f + 100.f, WindowHeight / 2.f + 90.f };
    quit_button_sprite.setPosition(yesPos);
    FloatRect yesBounds = quit_button_sprite.getGlobalBounds();
    if (yesBounds.contains(mousePos))
        quit_yes_text.setFillColor(Color(255, 255, 0));
    else
        quit_yes_text.setFillColor(Color(180, 255, 100));
    quit_button_sprite.setPosition(noPos);
    FloatRect noBounds = quit_button_sprite.getGlobalBounds();
    if (noBounds.contains(mousePos))
        quit_no_text.setFillColor(Color(255, 255, 0));
    else
        quit_no_text.setFillColor(Color(180, 255, 100));
}

void DrawQuit()
{
    DrawMainMenuBackground();
    window.draw(quit_bg_sprite);
    window.draw(quit_icon_sprite);
    window.draw(quit_title_text);
    window.draw(quit_title_text2);
    Vector2f yesPos = quit_yes_text.getPosition();
    if (quit_yes_text.getFillColor() == Color(255, 255, 0))
    {
        quit_button_hover_sprite.setPosition(yesPos);
        window.draw(quit_button_hover_sprite);
    }
    else
    {
        quit_button_sprite.setPosition(yesPos);
        window.draw(quit_button_sprite);
    }
    window.draw(quit_yes_text);
    Vector2f noPos = quit_no_text.getPosition();
    if (quit_no_text.getFillColor() == Color(255, 255, 0))
    {
        quit_button_hover_sprite.setPosition(noPos);
        window.draw(quit_button_hover_sprite);
    }
    else
    {
        quit_button_sprite.setPosition(noPos);
        window.draw(quit_button_sprite);
    }
    window.draw(quit_no_text);
    window.display();
}

void FadeOutToBlack()
{
    fadeAlpha = 0.f;
    fadeClock.restart();
    while (fadeAlpha < 255.f)
    {
        fadeAlpha = std::min(255.f, fadeClock.getElapsedTime().asSeconds() * 800.f);
        fadeRect.setFillColor(Color(0, 0, 0, (uint8_t)fadeAlpha));
        window.setView(view);
        UpdateMainMenuFish();
        DrawMainMenuBackground();
        window.draw(startgamebutton);
        window.draw(timeattackbutton);
        window.draw(highscorebutton);
        window.draw(optionsbutton);
        window.draw(quitbutton);
        window.draw(switchuserbutton);
        window.draw(creditsbutton);
        window.draw(logosp);
        window.draw(fadeRect);
        window.display();
    }
}

void FadeInFromBlack()
{
    fadeAlpha = 255.f;
    fadeClock.restart();
    while (fadeAlpha > 0.f)
    {
        fadeAlpha = std::max(0.f, 255.f - fadeClock.getElapsedTime().asSeconds() * 800.f);
        fadeRect.setFillColor(Color(0, 0, 0, (uint8_t)fadeAlpha));
        window.setView(view);
        UpdateMainMenuFish();
        DrawMainMenuBackground();
        window.draw(startgamebutton);
        window.draw(timeattackbutton);
        window.draw(highscorebutton);
        window.draw(optionsbutton);
        window.draw(quitbutton);
        window.draw(switchuserbutton);
        window.draw(creditsbutton);
        window.draw(logosp);
        window.draw(fadeRect);
        window.display();
    }
}

// ================= PROCEDURAL FUNCTIONS =================

void proceduralSort(HighScoreEntry arr[])
{
    for (int i = 0; i < MAX_SCORES - 1; i++)
    {
        for (int j = 0; j < MAX_SCORES - i - 1; j++)
        {
            if (arr[j].score < arr[j + 1].score)
            {
                HighScoreEntry temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

string formatScore(int s)
{
    string formatted = to_string(s);
    int insertPosition = static_cast<int>(formatted.length()) - 3;
    while (insertPosition > 0)
    {
        formatted.insert(insertPosition, ",");
        insertPosition -= 3;
    }
    return formatted;
}

void saveAllScores()
{
    ofstream f1("highscore_story.txt");
    if (f1.is_open())
        for (int i = 0; i < MAX_SCORES; i++)
            f1 << story_scores[i].name << "\n" << story_scores[i].score << "\n";
    ofstream f2("highscore_timeattack.txt");
    if (f2.is_open())
        for (int i = 0; i < MAX_SCORES; i++)
            f2 << timeattack_scores[i].name << "\n" << timeattack_scores[i].score << "\n";
}

void loadAllScores()
{
    auto loadFile = [](string filename, HighScoreEntry arr[], string defName)
        {
            ifstream file(filename);
            int count = 0;
            if (file.is_open())
                while (count < MAX_SCORES && getline(file, arr[count].name) && file >> arr[count].score)
                {
                    file.ignore();
                    count++;
                }
            for (int i = count; i < MAX_SCORES; i++)
            {
                arr[i].name = defName;
                arr[i].score = (MAX_SCORES - i) * 5000;
            }
        };
    loadFile("highscore_story.txt", story_scores, "Mr. Minnow");
    loadFile("highscore_timeattack.txt", timeattack_scores, "Speedy");
    proceduralSort(story_scores);
    proceduralSort(timeattack_scores);
}

void updateHighScoreTexts()
{
    HighScoreEntry* activeArr = (currentMode == STORY) ? story_scores : timeattack_scores;
    for (int i = 0; i < VISIBLE_SCORES; i++)
    {
        int idx = scrollOffset + i;
        textHSListRanks[i].setString(to_string(idx + 1) + ". ");
        textHSListNames[i].setString(activeArr[idx].name);
        textHSListScores[i].setString(formatScore(activeArr[idx].score));
        FloatRect sb = textHSListScores[i].getLocalBounds();
        textHSListScores[i].setOrigin({ sb.size.x, 0.f });
        textHSListScores[i].setPosition({ 540.f, LIST_START_Y + (i * LIST_SPACING) });
    }
}

void resetScores()
{
    string dName = (currentMode == STORY) ? "Mr. Minnow" : "Speedy";
    HighScoreEntry* activeArr = (currentMode == STORY) ? story_scores : timeattack_scores;
    for (int i = 0; i < MAX_SCORES; i++)
        activeArr[i] = { dName, (MAX_SCORES - i) * 5000 };
    scrollOffset = 0;
    updateHighScoreTexts();
    saveAllScores();
}

void Highscore()
{
    StartHighscore();
    fadeAlpha = 255.f;
    fadeClock.restart();
    while (fadeAlpha > 0.f)
    {
        fadeAlpha = std::max(0.f, 255.f - fadeClock.getElapsedTime().asSeconds() * 800.f);
        fadeRect.setFillColor(Color(0, 0, 0, (uint8_t)fadeAlpha));
        window.setView(view);
        DrawMainMenuBackground();
        window.draw(textHSMainTitle);
        window.draw(textHSStoryMode);
        window.draw(HSModeUnderline);
        window.draw(sprHSStoryArrowLeft);
        window.draw(sprHSStoryArrowRight);
        window.draw(sprHSListArrowUp);
        window.draw(sprHSListArrowDown);
        for (int i = 0; i < VISIBLE_SCORES; i++)
        {
            window.draw(textHSListRanks[i]);
            window.draw(textHSListNames[i]);
            window.draw(textHSListScores[i]);
        }
        window.draw(sprHSDonePlank);
        window.draw(textHSDoneButton);
        window.draw(sprHSResetPlank);
        window.draw(textHSResetButton);
        window.draw(fadeRect);
        window.display();
    }
    Clock frameClock;
    while (window.isOpen())
    {
        float dt = frameClock.restart().asSeconds();
        totaltime += dt;
        BarracudaFishanimation();
        QueenTriggerFish();
        for (auto& obj : smallfishs)
        {
            obj.shape.setPosition({ obj.sprite.getPosition().x, obj.sprite.getPosition().y });
            obj.update(286, 126);
            float posX = obj.sprite.getPosition().x;
            if (posX <= -150.f || posX >= WindowWidth + 150.f)
            {
                obj.velocityX_AXIS *= -1;
            }
        }
        UpdateHighscore();
        DrawHighscore();
    }
}

void addNewHighScore(string name, int score, bool isStoryMode)
{
    HighScoreEntry* activeList = isStoryMode ? story_scores : timeattack_scores;
    if (score < activeList[MAX_SCORES - 1].score)
        return;
    activeList[MAX_SCORES - 1].name = name;
    activeList[MAX_SCORES - 1].score = score;
    proceduralSort(activeList);
    saveAllScores();
    updateHighScoreTexts();
}

void StartHighscore()
{
    window.setFramerateLimit(60);
    window.setView(view);
    texHSDoneNormal.setSmooth(true);
    texHSDoneHover.setSmooth(true);
    texHSResetNormal.setSmooth(true);
    texHSResetHover.setSmooth(true);
    texHSArrowNormal.setSmooth(true);
    texHSArrowHover.setSmooth(true);
    texHSArrowDown.setSmooth(true);
    sprHSDonePlank.setTexture(texHSDoneNormal);
    sprHSDonePlank.setOrigin({ sprHSDonePlank.getLocalBounds().size.x / 2.f, sprHSDonePlank.getLocalBounds().size.y / 2.f });
    sprHSDonePlank.setPosition({ 400.f, 470.f });
    sprHSResetPlank.setTexture(texHSResetNormal);
    sprHSResetPlank.setOrigin({ sprHSResetPlank.getLocalBounds().size.x / 2.f, sprHSResetPlank.getLocalBounds().size.y / 2.f });
    sprHSResetPlank.setPosition({ 400.f, 560.f });
    sprHSStoryArrowRight.setTexture(texHSArrowNormal);
    sprHSStoryArrowRight.setOrigin({ texHSArrowNormal.getSize().x / 2.f, texHSArrowNormal.getSize().y / 2.f });
    sprHSStoryArrowRight.setPosition({ 480.f, 130.f });
    sprHSStoryArrowLeft.setTexture(texHSArrowNormal);
    sprHSStoryArrowLeft.setOrigin({ texHSArrowNormal.getSize().x / 2.f, texHSArrowNormal.getSize().y / 2.f });
    sprHSStoryArrowLeft.setPosition({ 320.f, 130.f });
    sprHSStoryArrowLeft.setScale({ -1.f, 1.f });
    sprHSListArrowUp.setTexture(texHSArrowNormal);
    sprHSListArrowUp.setOrigin({ texHSArrowNormal.getSize().x / 2.f, texHSArrowNormal.getSize().y / 2.f });
    sprHSListArrowUp.setPosition({ 570.f, LIST_START_Y + 10.f });
    sprHSListArrowUp.setRotation(degrees(270.f));
    sprHSListArrowDown.setTexture(texHSArrowNormal);
    sprHSListArrowDown.setOrigin({ texHSArrowNormal.getSize().x / 2.f, texHSArrowNormal.getSize().y / 2.f });
    sprHSListArrowDown.setPosition({ 570.f, LIST_START_Y + (9 * LIST_SPACING) + 10.f });
    sprHSListArrowDown.setRotation(degrees(90.f));
    textHSMainTitle.setFillColor(colorTitleGreen);
    textHSMainTitle.setOutlineThickness(2.f);
    textHSMainTitle.setOrigin({ textHSMainTitle.getLocalBounds().size.x / 2.f, 0.f });
    textHSMainTitle.setPosition({ 400.f, 40.f });
    textHSStoryMode.setOrigin({ textHSStoryMode.getLocalBounds().size.x / 2.f, 0.f });
    textHSStoryMode.setPosition({ 400.f, 115.f });
    HSModeUnderline.setFillColor(Color::White);
    HSModeUnderline.setSize({ textHSStoryMode.getLocalBounds().size.x, 2.f });
    HSModeUnderline.setOrigin({ HSModeUnderline.getSize().x / 2.f, 0.f });
    HSModeUnderline.setPosition({ 400.f, 142.f });
    for (int i = 0; i < VISIBLE_SCORES; i++)
    {
        textHSListRanks[i].setFillColor(colorListText);
        textHSListNames[i].setFillColor(colorListText);
        textHSListScores[i].setFillColor(colorListText);
        textHSListRanks[i].setPosition({ 260.f, LIST_START_Y + (i * LIST_SPACING) });
        textHSListNames[i].setPosition({ 290.f, LIST_START_Y + (i * LIST_SPACING) });
    }
    textHSDoneButton.setFillColor(colorDoneText);
    textHSDoneButton.setOrigin({ textHSDoneButton.getLocalBounds().size.x / 2.f, textHSDoneButton.getLocalBounds().size.y / 2.f });
    textHSDoneButton.setOutlineThickness(2.f);
    textHSDoneButton.setPosition({ 400.f, 461.f });
    textHSResetButton.setFillColor(colorResetText);
    textHSResetButton.setOrigin({ textHSResetButton.getLocalBounds().size.x / 2.f, textHSResetButton.getLocalBounds().size.y / 2.f });
    textHSResetButton.setPosition({ 400.f, 557.f });
    loadAllScores();
    updateHighScoreTexts();
}

void UpdateHighscore()
{
    PlayingSound(false);
    Vector2f mousePos = window.mapPixelToCoords(Mouse::getPosition(window), view);
    bool mouseClicked = false;
    while (const optional event = window.pollEvent())
    {
        if (event->is<Event::Closed>())
            window.close();
        else if (const auto* resizeEvent = event->getIf<Event::Resized>())
        {
            view.setViewport(FloatRect({ 0.f, 0.f }, { 1.f, 1.f }));
            window.setView(view);
        }
        if (const auto* mouseBtn = event->getIf<Event::MouseButtonReleased>())
            if (mouseBtn->button == Mouse::Button::Left)
                mouseClicked = true;
    }
    static Clock arrowHoldClock;
    static bool wasMousePressed = false;
    bool isMousePressed = Mouse::isButtonPressed(Mouse::Button::Left);
    bool triggerScroll = false;
    if (isMousePressed)
    {
        if (!wasMousePressed)
        {
            triggerScroll = true;
            arrowHoldClock.restart();
        }
        else if (arrowHoldClock.getElapsedTime().asSeconds() > 0.15f)
        {
            triggerScroll = true;
            arrowHoldClock.restart();
        }
    }
    wasMousePressed = isMousePressed;
    auto processArrow = [&](Sprite& s, bool action)
        {
            if (s.getGlobalBounds().contains(mousePos))
            {
                if (isMousePressed)
                {
                    s.setTexture(texHSArrowDown);
                    s.setOrigin({ texHSArrowDown.getSize().x / 2.f, texHSArrowDown.getSize().y / 2.f });
                }
                else
                {
                    s.setTexture(texHSArrowHover);
                    s.setOrigin({ texHSArrowHover.getSize().x / 2.f, texHSArrowHover.getSize().y / 2.f });
                }
                return action;
            }
            else
            {
                s.setTexture(texHSArrowNormal);
                s.setOrigin({ texHSArrowNormal.getSize().x / 2.f, texHSArrowNormal.getSize().y / 2.f });
                return false;
            }
        };
    if (processArrow(sprHSListArrowUp, triggerScroll) && scrollOffset > 0)
    {
        scrollOffset--;
        updateHighScoreTexts();
    }
    if (processArrow(sprHSListArrowDown, triggerScroll) && scrollOffset < MAX_SCORES - VISIBLE_SCORES)
    {
        scrollOffset++;
        updateHighScoreTexts();
    }
    if (processArrow(sprHSStoryArrowLeft, mouseClicked) || processArrow(sprHSStoryArrowRight, mouseClicked))
    {
        currentMode = (currentMode == STORY) ? TIME_ATTACK : STORY;
        textHSStoryMode.setString((currentMode == STORY) ? "Story Mode" : "Time Attack");
        textHSStoryMode.setOrigin({ textHSStoryMode.getLocalBounds().size.x / 2.f, 0.f });
        HSModeUnderline.setSize({ textHSStoryMode.getLocalBounds().size.x, 2.f });
        HSModeUnderline.setOrigin({ HSModeUnderline.getSize().x / 2.f, 0.f });
        scrollOffset = 0;
        updateHighScoreTexts();
    }
    if (sprHSDonePlank.getGlobalBounds().contains(mousePos))
    {
        sprHSDonePlank.setTexture(texHSDoneHover);
        textHSDoneButton.setFillColor(colorHoverHighlight);
        if (mouseClicked)
        {
            window.setView(view);
            MainMenu();
        }
    }
    else
    {
        sprHSDonePlank.setTexture(texHSDoneNormal);
        textHSDoneButton.setFillColor(colorDoneText);
    }
    if (sprHSResetPlank.getGlobalBounds().contains(mousePos))
    {
        sprHSResetPlank.setTexture(texHSResetHover);
        textHSResetButton.setFillColor(colorHoverHighlight);
        if (mouseClicked)
            resetScores();
    }
    else
    {
        sprHSResetPlank.setTexture(texHSResetNormal);
        textHSResetButton.setFillColor(colorResetText);
    }
}

void DrawHighscore()
{
    window.setView(view);
    DrawMainMenuBackground();
    window.draw(textHSMainTitle);
    window.draw(textHSStoryMode);
    window.draw(HSModeUnderline);
    window.draw(sprHSStoryArrowLeft);
    window.draw(sprHSStoryArrowRight);
    window.draw(sprHSListArrowUp);
    window.draw(sprHSListArrowDown);
    for (int i = 0; i < VISIBLE_SCORES; i++)
    {
        window.draw(textHSListRanks[i]);
        window.draw(textHSListNames[i]);
        window.draw(textHSListScores[i]);
    }
    window.draw(sprHSDonePlank);
    window.draw(textHSDoneButton);
    window.draw(sprHSResetPlank);
    window.draw(textHSResetButton);
    window.display();
}

void Credits()
{
    StartCredits();
    fadeAlpha = 255.f;
    fadeClock.restart();
    while (fadeAlpha > 0.f)
    {
        fadeAlpha = std::max(0.f, 255.f - fadeClock.getElapsedTime().asSeconds() * 800.f);
        fadeRect.setFillColor(Color(0, 0, 0, (uint8_t)fadeAlpha));
        window.setView(view);
        DrawCredits();
        window.draw(fadeRect);
        window.display();
    }
    Clock frameClock;
    while (window.isOpen())
    {
        float dt = frameClock.restart().asSeconds();
        totaltime += dt;
        BarracudaFishanimation();
        QueenTriggerFish();
        for (auto& obj : smallfishs)
        {
            obj.shape.setPosition({ obj.sprite.getPosition().x, obj.sprite.getPosition().y });
            obj.update(286, 126);
            float posX = obj.sprite.getPosition().x;
            if (posX <= -150.f || posX >= WindowWidth + 150.f)
            {
                obj.velocityX_AXIS *= -1;
            }
        }
        if (UpdateCredits())
            break;
        window.setView(view);
        DrawCredits();
        window.display();
    }
    fadeAlpha = 0.f;
    fadeClock.restart();
    while (fadeAlpha < 255.f)
    {
        fadeAlpha = std::min(255.f, fadeClock.getElapsedTime().asSeconds() * 800.f);
        fadeRect.setFillColor(Color(0, 0, 0, (uint8_t)fadeAlpha));
        window.setView(view);
        DrawCredits();
        window.draw(fadeRect);
        window.display();
    }
}

void StartCredits()
{
    window.setFramerateLimit(60);
    window.setView(view);
    creditsText.emplace(fontcredits);
    fontcredits.setSmooth(true);
    texHSDoneNormalcredits.setSmooth(true);
    texHSDoneHovercredits.setSmooth(true);
    creditsText->setCharacterSize(28);
    creditsText->setOutlineThickness(1.f);
    creditsText->setOutlineColor(Color(0, 0, 0));
    creditsText->setFillColor(Color(255, 255, 255));
    creditsText->setString(
        "Team Members:\n\n\n\n"
        "1- Loay Mohamed\n"
        "2- Zeyad Khaled\n"
        "3- Mohamed Saraya\n"
        "4- Mohamed El Sayed\n"
        "5- Zeyad Gouda\n"
        "6- Mohamed Yasser\n"
        "7- Mohamed Medhat\n\n\n"
        "THANKS FOR PLAYING!");
    FloatRect bounds = creditsText->getLocalBounds();
    creditsText->setOrigin({ bounds.position.x + bounds.size.x / 2.f, 0.f });
    creditsText->setPosition({ WindowWidth / 2.f, WindowHeight / 2.f - 260.f });
    sprHSDonePlankcredits.setPosition({ 400.f, 545.f });
    sprHSDonePlankcredits.setOrigin({ texHSDoneNormalcredits.getSize().x / 2.f, texHSDoneNormalcredits.getSize().y / 2.f });
    textHSDoneButtoncredits.setPosition({ 400.f, 536.f });
    textHSDoneButtoncredits.setOutlineThickness(2.f);
    FloatRect doneBounds = textHSDoneButtoncredits.getLocalBounds();
    textHSDoneButtoncredits.setOrigin({ doneBounds.size.x / 2.f, doneBounds.size.y / 2.f });
}

bool UpdateCredits()
{
    PlayingSound(false);
    Vector2f mousePos = window.mapPixelToCoords(Mouse::getPosition(window), view);
    while (const optional event = window.pollEvent())
    {
        if (event->is<Event::Closed>())
        {
            window.close();
            return true;
        }
        if (const auto* keyPressed = event->getIf<Event::KeyPressed>())
        {
            if (keyPressed->code == Keyboard::Key::Escape)
                return true;
        }
        if (const auto* mouseBtn = event->getIf<Event::MouseButtonReleased>())
        {
            if (mouseBtn->button == Mouse::Button::Left)
            {
                if (sprHSDonePlankcredits.getGlobalBounds().contains(mousePos))
                    return true;
            }
        }
    }
    if (sprHSDonePlankcredits.getGlobalBounds().contains(mousePos))
    {
        sprHSDonePlankcredits.setTexture(texHSDoneHovercredits);
        textHSDoneButtoncredits.setFillColor(Color(255, 255, 0));
    }
    else
    {
        sprHSDonePlankcredits.setTexture(texHSDoneNormalcredits);
        textHSDoneButtoncredits.setFillColor(Color(180, 255, 100));
    }
    return false;
}

void DrawCredits()
{
    DrawMainMenuBackground();
    if (creditsText.has_value())
        window.draw(*creditsText);
    window.draw(sprHSDonePlankcredits);
    window.draw(textHSDoneButtoncredits);
}

// ==========================================
// ===== Select Level =====
// ==========================================

// load all assets & init texts/pearls
void StartSelectLevel()
{
    isLoading = false;
    loadProgress = 0.f;

    if (!bgTexture.loadFromFile("C:/Users/MOHAMED/source/repos/test1/x64/Debug/Assets/Select_level/gamemap_bg.jpeg"))
        std::cerr << "Error: Could not load gamemap_bg.jpeg" << std::endl;
    bgSprite.setTexture(bgTexture);
    bgTexture.setSmooth(true);

    if (!signTexture.loadFromFile("C:/Users/MOHAMED/source/repos/test1/x64/Debug/Assets/Select_level/gamemap_tittleboard.jpeg"))
        std::cerr << "Error: Could not load gamemap_tittleboard.jpeg" << std::endl;
    signTexture.setSmooth(true);
    signSprite.setTexture(signTexture);
    signSprite.setPosition({ 10.f, 10.f });

    if (!bannerTexture.loadFromFile("C:/Users/MOHAMED/source/repos/test1/x64/Debug/Assets/Select_level/bord.jpeg"))
        std::cerr << "Error: Could not load bord.jpeg" << std::endl;
    bannerTexture.setSmooth(true);
    bannerSprite.setTexture(bannerTexture);
    bannerSprite.setPosition({ 1.f, 445.f });

    if (!myTexture.loadFromFile("C:/Users/MOHAMED/source/repos/test1/x64/Debug/Assets/Select_level/shell_tinybtn124_normal.jpeg"))
        std::cerr << "Error: Could not load shell_tinybtn124_normal.jpeg" << std::endl;
    if (!myHoverTexture.loadFromFile("C:/Users/MOHAMED/source/repos/test1/x64/Debug/Assets/Select_level/shell_tinybtn124_high.jpeg"))
        std::cerr << "Error: Could not load shell_tinybtn124_high.jpeg" << std::endl;
    myTexture.setSmooth(true); myHoverTexture.setSmooth(true);
    mySprite.setTexture(myTexture);
    mySprite.setPosition({ 350.f, 565.f });

    if (!pearlTexture.loadFromFile("C:/Users/MOHAMED/source/repos/test1/x64/Debug/Assets/Select_level/white.jpeg"))
        std::cerr << "Error: Could not load white.jpeg" << std::endl;
    pearlTexture.setSmooth(true);
    if (!pearlUnlockedTexture.loadFromFile("C:/Users/MOHAMED/source/repos/test1/x64/Debug/Assets/Select_level/gold.jpeg"))
        std::cerr << "Error: Could not load gold.jpeg" << std::endl;
    pearlUnlockedTexture.setSmooth(true);
    if (!loadBarTexture.loadFromFile("C:/Users/MOHAMED/source/repos/test1/x64/Debug/Assets/Select_level/gamemap_loadbar.jpeg"))
        std::cerr << "Error: Could not load gamemap_loadbar.jpeg" << std::endl;
    loadBarTexture.setSmooth(true);

    if (!font.openFromFile("C:/Users/MOHAMED/source/repos/test1/x64/Debug/Assets/Fonts/ARIALNBI.ttf"))
        cout << "Error loading font!\n";
    font.setSmooth(true);

    // init texts
    staticTxt = Text(font, "NEW GAME", 25);   staticTxt.setPosition({ 356.f, 493.f });
    menuTxt = Text(font, "menu", 20);        menuTxt.setPosition({ 390.f, 570.f });
    levelTxt = Text(font, "choose level", 24); centerText(levelTxt);
    loadingtxt = Text(font, "Now Loading...", 24); loadingtxt.setPosition({ 356.f, 493.f });

    // init pearls
    pearls[0] = { &pearlSprite1, 25.f, "Level 1", &level1Unlocked, {205.f, 75.f},  {0.13f, 0.13f}, {220.f, 100.f}, {0.13f, 0.13f} };
    pearls[1] = { &pearlSprite2, 25.f, "Level 2", &level2Unlocked, {235.f, 125.f}, {0.15f, 0.15f}, {248.f, 148.f}, {0.15f, 0.15f} };
    pearls[2] = { &pearlSprite3, 25.f, "Level 3", &level3Unlocked, {230.f, 165.f}, {0.15f, 0.15f}, {242.f, 188.f}, {0.15f, 0.15f} };
}

// center text horizontally on screen
void centerText(Text& text)
{
    FloatRect b = text.getLocalBounds();
    text.setOrigin({ b.position.x + b.size.x / 2.f, b.position.y + b.size.y / 2.f });
    text.setPosition({ 415.f, 545.f });
}

// update pearl states, button hover, load bar progress
void UpdateSelectLevel(float dt)
{
    PearlData* activePearls = isTimeAttackMode ? ta_pearls : pearls;
    PlayingSound(true);
    Vector2f mf = window.mapPixelToCoords(Mouse::getPosition(window), view);
    levelTxt.setString("choose level");
    centerText(levelTxt);

    static bool pearlClicked = false;

    // set texture/position per pearl & show name on hover
    for (int i = 0; i < 3; ++i)
    {
        auto& p = activePearls[i];
        if (*p.unlocked)
        {
            p.sprite->setTexture(pearlUnlockedTexture);
            p.sprite->setPosition(p.unlockedPos);
            p.sprite->setScale(p.unlockedScale);
        }
        else
        {
            p.sprite->setTexture(pearlTexture);
            p.sprite->setPosition(p.lockedPos);
            p.sprite->setScale(p.lockedScale);
        }
        FloatRect bounds = p.sprite->getGlobalBounds();
        Vector2f center = { bounds.position.x + bounds.size.x / 2.f, bounds.position.y + bounds.size.y / 2.f };
        if (hypot(mf.x - center.x, mf.y - center.y) <= p.radius)
        {
            levelTxt.setString(p.name);
            centerText(levelTxt);
        }
    }

    // back button hover
    if (!pearlClicked)
    {
        if (mySprite.getGlobalBounds().contains(mf))
        {
            mySprite.setTexture(myHoverTexture);
            levelTxt.setString("back to menu");
            centerText(levelTxt);
        }
        else mySprite.setTexture(myTexture);
    }
    else mySprite.setPosition({ -1000.f, -1000.f });

    // advance load bar; on complete → transition to game
    if (isLoading)
    {
        loadProgress += barSpeed * dt;
        if (loadProgress >= barWidth)
        {
            loadProgress = barWidth;
            DrawSelectLevel();
            FadeOutToBlack();
            GameScreen(selectedLevel);
            FadeInFromBlack();
            isLoading = false;
            loadProgress = 0.f;
            pearlClicked = false;
            return;
        }
    }
}

// draw all select-level elements
void DrawSelectLevel()
{
    window.setView(view);
    window.clear();
    window.draw(bgSprite);
    window.draw(signSprite);
    window.draw(bannerSprite);

    // pearls (normal or time attack)
    if (!isTimeAttackMode) { window.draw(pearlSprite1); window.draw(pearlSprite2); window.draw(pearlSprite3); }
    else { window.draw(ta_pearlSprite1); window.draw(ta_pearlSprite2); window.draw(ta_pearlSprite3); }

    window.draw(mySprite);
    window.draw(menuTxt);
    window.draw(levelTxt);

    // load bar OR static "NEW GAME" text
    if (isLoading)
    {
        Sprite bar(loadBarTexture);
        bar.setPosition({ barX, barY });
        float sx = barWidth / (float)loadBarTexture.getSize().x;
        float sy = barHeight / (float)loadBarTexture.getSize().y;
        bar.setScale({ sx, sy });
        bar.setTextureRect(IntRect({ 0, 0 }, { (int)(loadProgress / sx), (int)loadBarTexture.getSize().y }));
        window.draw(bar);
        window.draw(loadingtxt);
    }
    else window.draw(staticTxt);

    window.display();
}

// main select-level loop
void Select_level()
{
    StartSelectLevel();
    Clock clock;
    while (window.isOpen())
    {
        float dt = clock.restart().asSeconds();
        while (auto event = window.pollEvent())
        {
            if (event->is<Event::Closed>()) window.close();
            else if (const auto* resizeEvent = event->getIf<Event::Resized>())
            {
                view.setViewport(FloatRect({ 0.f, 0.f }, { 1.f, 1.f }));
                window.setView(view);
            }

            // click: back button or unlocked pearl
            if (auto mouseEvent = event->getIf<Event::MouseButtonReleased>())
                if (mouseEvent->button == Mouse::Button::Left)
                {
                    if (isLoading) continue;
                    Vector2f mf = window.mapPixelToCoords(Mouse::getPosition(window), view);
                    if (mySprite.getGlobalBounds().contains(mf)) return;

                    PearlData* activePearls = isTimeAttackMode ? ta_pearls : pearls;
                    for (int i = 0; i < 3; ++i)
                    {
                        if (*activePearls[i].unlocked)
                        {
                            FloatRect bounds = activePearls[i].sprite->getGlobalBounds();
                            Vector2f center = { bounds.position.x + bounds.size.x / 2.f,
                                               bounds.position.y + bounds.size.y / 2.f };
                            if (hypot(mf.x - center.x, mf.y - center.y) <= activePearls[i].radius)
                            {
                                isLoading = true;
                                loadProgress = 0.f;
                                selectedLevel = i + 1;
                            }
                        }
                    }
                }
        }
        window.setView(view);
        UpdateSelectLevel(dt);
        DrawSelectLevel();
    }
}


// ==========================================
// ===== Game Screen =====
// ==========================================

// load bg, buttons, quit popup & level-specific assets
void StartGameScreen(int level)
{
    selectedLevel = level;
    gameScreenActive = true;
    window.setFramerateLimit(60);
    window.setView(view);

    // background
    if (!gameScreenBgTexture.loadFromFile(
        "C:/Users/MOHAMED/source/repos/test1/x64/Debug/Assets/GameScreen/shell_stageinfo.jpg"))
        cout << "Error: shell_stageinfo.jpg\n";
    gameScreenBgTexture.setSmooth(true);
    gameScreenBgSprite = sf::Sprite(gameScreenBgTexture);
    sf::Vector2u bgSize = gameScreenBgTexture.getSize();
    gameScreenBgSprite.setScale({ WindowWidth / (float)bgSize.x, WindowHeight / (float)bgSize.y });
    gameScreenBgSprite.setPosition({ 0.f, 0.f });

    // options / quit / continue buttons
    struct BtnInfo { const char* n; const char* h; float x, y, sc, hsc, hw, hh; };
    BtnInfo btnInfos[3] = {
        {"C:/Users/MOHAMED/source/repos/test1/x64/Debug/Assets/GameScreen/option1.png",
         "C:/Users/MOHAMED/source/repos/test1/x64/Debug/Assets/GameScreen/option2.png",
         180.f, 560.f, 1 / 10.f, 1 / 10.f, 0.f, 0.f},
        {"C:/Users/MOHAMED/source/repos/test1/x64/Debug/Assets/GameScreen/quit1.png",
         "C:/Users/MOHAMED/source/repos/test1/x64/Debug/Assets/GameScreen/quit2.png",
         630.f, 560.f, 2 / 10.f, 2 / 10.f, 0.f, 0.f},
        {"C:/Users/MOHAMED/source/repos/test1/x64/Debug/Assets/GameScreen/continue1.png",
         "C:/Users/MOHAMED/source/repos/test1/x64/Debug/Assets/GameScreen/continue_large-removebg-preview.png",
         400.f, 540.f, 1.5f / 10.f, 3.5f / 10.f, 80.f, 30.f},
    };
    for (int i = 0; i < 3; i++)
    {
        if (!gsBtns[i].normalTex.loadFromFile(btnInfos[i].n)) cout << "Error btn normal " << i << "\n";
        if (!gsBtns[i].hoverTex.loadFromFile(btnInfos[i].h))  cout << "Error btn hover " << i << "\n";
        gsBtns[i].normalTex.setSmooth(true); gsBtns[i].hoverTex.setSmooth(true);
        gsBtns[i].x = btnInfos[i].x; gsBtns[i].y = btnInfos[i].y;
        gsBtns[i].scale = btnInfos[i].sc; gsBtns[i].hoverScale = btnInfos[i].hsc;
        gsBtns[i].hoverHalfW = btnInfos[i].hw; gsBtns[i].hoverHalfH = btnInfos[i].hh;
        gsBtns[i].sprite = sf::Sprite(gsBtns[i].normalTex);
        gsBtns[i].sprite->setOrigin({ gsBtns[i].normalTex.getSize().x / 2.f,
                                       gsBtns[i].normalTex.getSize().y / 2.f });
        gsBtns[i].sprite->setPosition({ gsBtns[i].x, gsBtns[i].y });
        gsBtns[i].sprite->setScale({ gsBtns[i].scale, gsBtns[i].scale });
    }

    // quit popup - bg, title, texts
    showQuitPopup = false;
    if (!quitPopup.bgTex.loadFromFile(
        "C:/Users/MOHAMED/source/repos/test1/x64/Debug/Assets/GameScreen/shell_shared_popupbg_medium-removebg-preview (2).png"))
        ;
    quitPopup.bgTex.setSmooth(true);
    quitPopup.bgSprite = sf::Sprite(quitPopup.bgTex);
    quitPopup.bgSprite->setOrigin({ quitPopup.bgTex.getSize().x / 2.f, quitPopup.bgTex.getSize().y / 2.f });
    quitPopup.bgSprite->setPosition({ QUIT_POPUP_X, QUIT_POPUP_Y });

    if (!quitPopup.titleTex.loadFromFile(
        "C:/Users/MOHAMED/source/repos/test1/x64/Debug/Assets/GameScreen/New Project (1).png"))
        ;
    quitPopup.titleTex.setSmooth(true);
    quitPopup.titleSprite = sf::Sprite(quitPopup.titleTex);
    quitPopup.titleSprite->setOrigin({ quitPopup.titleTex.getSize().x / 2.f, quitPopup.titleTex.getSize().y / 2.f });
    quitPopup.titleSprite->setPosition({ QUIT_POPUP_X, 180.f });
    quitPopup.titleSprite->setScale({ 0.5f, 0.5f });

    quitPopupLine1 = Text(font, "Are you sure you want to quit?", 22);
    quitPopupLine1.setFillColor(Color::White);
    { FloatRect b = quitPopupLine1.getLocalBounds(); quitPopupLine1.setOrigin({ b.size.x / 2.f, b.size.y / 2.f }); quitPopupLine1.setPosition({ QUIT_POPUP_X, 230.f }); }

    quitPopupLine2 = Text(font, "(Select quit option)", 18);
    quitPopupLine2.setFillColor(Color(200, 230, 200));
    { FloatRect b = quitPopupLine2.getLocalBounds(); quitPopupLine2.setOrigin({ b.size.x / 2.f, b.size.y / 2.f }); quitPopupLine2.setPosition({ QUIT_POPUP_X, 250.f }); }

    // quit popup buttons
    struct PopupBtnInfo { const char* n; const char* h; float y; };
    PopupBtnInfo pbInfos[3] = {
        {"C:/Users/MOHAMED/source/repos/test1/x64/Debug/Assets/GameScreen/Screenshot_2026-04-25_222549-removebg-preview.png",
         "C:/Users/MOHAMED/source/repos/test1/x64/Debug/Assets/GameScreen/Screenshot_2026-04-25_164334-removebg-preview.png", QUIT_BTN1_Y},
        {"C:/Users/MOHAMED/source/repos/test1/x64/Debug/Assets/GameScreen/Screenshot_2026-04-25_223151-removebg-preview.png",
         "C:/Users/MOHAMED/source/repos/test1/x64/Debug/Assets/GameScreen/Screenshot_2026-04-25_230020-removebg-preview.png", QUIT_BTN2_Y},
        {"C:/Users/MOHAMED/source/repos/test1/x64/Debug/Assets/GameScreen/Screenshot_2026-04-25_223327-removebg-preview.png",
         "C:/Users/MOHAMED/source/repos/test1/x64/Debug/Assets/GameScreen/Screenshot_2026-04-25_230106-removebg-preview.png", QUIT_BTN3_Y},
    };
    for (int i = 0; i < 3; i++)
    {
        if (!quitPopup.btns[i].normalTex.loadFromFile(pbInfos[i].n)) cout << "Error popup btn normal " << i << "\n";
        if (!quitPopup.btns[i].hoverTex.loadFromFile(pbInfos[i].h))  cout << "Error popup btn hover " << i << "\n";
        quitPopup.btns[i].sprite = sf::Sprite(quitPopup.btns[i].normalTex);
        quitPopup.btns[i].sprite->setOrigin({ quitPopup.btns[i].normalTex.getSize().x / 2.f,
                                               quitPopup.btns[i].normalTex.getSize().y / 2.f });
        quitPopup.btns[i].sprite->setPosition({ QUIT_POPUP_X, pbInfos[i].y });
        quitPopup.btns[i].sprite->setScale({ QUIT_BTN_SCALE, QUIT_BTN_SCALE });
    }

    // level-specific images & texts
    int idx = level - 1;
    LevelData& ld = levels[idx];
    if (!ld.font.openFromFile("C:/Users/MOHAMED/source/repos/test1/x64/Debug/Assets/Fonts/trebuc.ttf"));
    ld.font.setSmooth(true);

    if (level == 1)
    {
        ld.imageCount = 4;
        struct ImgInfo { const char* path; float x, y, sc; };
        ImgInfo imgs[4] = {
            {"C:/Users/MOHAMED/source/repos/test1/x64/Debug/Assets/GameScreen/Screenshot_2026-04-25_021234-artguru-removebg-preview.png", 580.f, 230.f, 0.4f},
            {"C:/Users/MOHAMED/source/repos/test1/x64/Debug/Assets/GameScreen/Screenshot_2026-04-24_222635-removebg-preview.png",         300.f, 403.f, 0.9f},
            {"C:/Users/MOHAMED/source/repos/test1/x64/Debug/Assets/GameScreen/Screenshot_2026-04-24_222758-removebg-preview.png",         410.f, 405.f, 0.2f},
            {"C:/Users/MOHAMED/source/repos/test1/x64/Debug/Assets/GameScreen/hudimage-removebg.png",                                     480.f, 415.f, 0.6f},
        };
        for (int i = 0; i < 4; i++)
        {
            if (!ld.images[i].tex.loadFromFile(imgs[i].path)) cout << "Error lvl1 img " << i << "\n";
            ld.images[i].tex.setSmooth(true);
            ld.images[i].x = imgs[i].x; ld.images[i].y = imgs[i].y; ld.images[i].scale = imgs[i].sc;
            ld.images[i].sprite = sf::Sprite(ld.images[i].tex);
            ld.images[i].sprite->setOrigin({ ld.images[i].tex.getSize().x / 2.f, ld.images[i].tex.getSize().y / 2.f });
            ld.images[i].sprite->setPosition({ imgs[i].x, imgs[i].y });
            ld.images[i].sprite->setScale({ imgs[i].sc, imgs[i].sc });
        }

        ld.textCount = 6;
        struct TxtInfo { const char* str; int sz; Color col; float x, y; bool c; };
        TxtInfo txts[6] = {
            {"1. Level 1",                                    26, Color(255,220,50),  415.f, 55.f,  true},
            {"Time to get your fins wet!",                    16, Color(255,255,255), 415.f, 88.f,  true},
            {"Meet Boris the Butterfly Fish. Boris lives in the\nwarm waters of the Sandy Shoal - a beautiful\narea off the Frenzy Coast. Enjoy the sights, but\ndon't get too comfortable... A fish still needs his\nlunch, and it's a fish-eat-fish world out there!",
                                                              14, Color(240,240,240), 150.f, 145.f, false},
            {"How to play:",                                  17, Color(240,240,240), 145.f, 250.f, false},
            {"- Use your mouse to control Boris.\n- Eat fish that are smaller than you.\n- Avoid anything that's larger than you.\n- Eat enough fish and you'll grow bigger!",
                                                              13, Color(240,240,240), 160.f, 280.f, false},
            {"Warm up those chompers and have some fun!",     15, Color(240,240,240), 390.f, 480.f, true},
        };
        for (int i = 0; i < 6; i++)
        {
            ld.texts[i].text.emplace(ld.font);
            ld.texts[i].text->setString(txts[i].str);
            ld.texts[i].text->setCharacterSize(txts[i].sz);
            ld.texts[i].text->setFillColor(txts[i].col);
            ld.texts[i].text->setOutlineColor(Color(20, 20, 20));
            ld.texts[i].text->setOutlineThickness(i == 0 ? 2.f : 1.f);
            if (txts[i].c) { FloatRect b = ld.texts[i].text->getLocalBounds(); ld.texts[i].text->setOrigin({ b.size.x / 2.f, b.size.y / 2.f }); }
            ld.texts[i].text->setPosition({ txts[i].x, txts[i].y });
        }
    }
    else if (level == 2)
    {
        ld.imageCount = 4;
        struct ImgInfo { const char* path; float x, y, sc; };
        ImgInfo imgs[4] = {
            {"C:/Users/MOHAMED/source/repos/test1/x64/Debug/Assets/GameScreen/starbubble1.png",                                    580.f, 200.f, 0.9f},
            {"C:/Users/MOHAMED/source/repos/test1/x64/Debug/Assets/GameScreen/sparkletrail.png",                                  580.f, 240.f, 0.9f},
            {"C:/Users/MOHAMED/source/repos/test1/x64/Debug/Assets/GameScreen/scr_mouseleftclickarrow-removebg-preview.png",      320.f, 405.f, 0.9f},
            {"C:/Users/MOHAMED/source/repos/test1/x64/Debug/Assets/GameScreen/Screenshot_2026-04-24_223502-removebg-preview.png", 440.f, 415.f, 0.6f},
        };
        for (int i = 0; i < 4; i++)
        {
            if (!ld.images[i].tex.loadFromFile(imgs[i].path)) cout << "Error lvl2 img " << i << "\n";
            ld.images[i].tex.setSmooth(true);
            ld.images[i].sprite = sf::Sprite(ld.images[i].tex);
            ld.images[i].sprite->setOrigin({ ld.images[i].tex.getSize().x / 2.f, ld.images[i].tex.getSize().y / 2.f });
            ld.images[i].sprite->setPosition({ imgs[i].x, imgs[i].y });
            ld.images[i].sprite->setScale({ imgs[i].sc, imgs[i].sc });
        }

        ld.textCount = 7;
        struct TxtInfo { const char* str; int sz; Color col; float x, y; bool c; };
        TxtInfo txts[7] = {
            {"2. Level 2",                                    26, Color(255,220,50),  415.f, 55.f,  true},
            {"A Tasty Treat",                                 16, Color(255,255,255), 415.f, 88.f,  true},
            {"Watch for Starfish!",                           17, Color(240,240,240), 150.f, 145.f, false},
            {"Tasty STARFISH have been seen floating around\nthese parts. Grab a starfish bubble for some\nextra points. They won't help Boris get any\nbigger, but they're sure to grow your score!",
                                                              14, Color(240,240,240), 150.f, 175.f, false},
            {"How to Dash:",                                  17, Color(240,240,240), 150.f, 270.f, false},
            {"Tap the left mouse button for a short burst of speed.\nIt doesn't last long, but it might help you out\nof a tough spot.",
                                                              14, Color(240,240,240), 150.f, 300.f, false},
            {"Stay alert, and watch out for sharks!",         15, Color(240,240,240), 390.f, 480.f, true},
        };
        for (int i = 0; i < 7; i++)
        {
            ld.texts[i].text.emplace(ld.font);
            ld.texts[i].text->setString(txts[i].str);
            ld.texts[i].text->setCharacterSize(txts[i].sz);
            ld.texts[i].text->setFillColor(txts[i].col);
            ld.texts[i].text->setOutlineColor(Color(20, 20, 20));
            ld.texts[i].text->setOutlineThickness(i == 0 ? 2.f : 1.f);
            if (txts[i].c) { FloatRect b = ld.texts[i].text->getLocalBounds(); ld.texts[i].text->setOrigin({ b.size.x / 2.f, b.size.y / 2.f }); }
            ld.texts[i].text->setPosition({ txts[i].x, txts[i].y });
        }
    }
    else if (level == 3)
    {
        ld.imageCount = 3;
        struct ImgInfo { const char* path; float x, y, sc; };
        ImgInfo imgs[3] = {
            {"C:/Users/MOHAMED/source/repos/test1/x64/Debug/Assets/GameScreen/ffbubble1.png",                         250.f, 350.f, 1.0f},
            {"C:/Users/MOHAMED/source/repos/test1/x64/Debug/Assets/GameScreen/sparkletrail.png",                     250.f, 400.f, 0.8f},
            {"C:/Users/MOHAMED/source/repos/test1/x64/Debug/Assets/GameScreen/scr_frenzymeter-removebg-preview.png", 400.f, 280.f, 0.8f},
        };
        for (int i = 0; i < 3; i++)
        {
            if (!ld.images[i].tex.loadFromFile(imgs[i].path)) cout << "Error lvl3 img " << i << "\n";
            ld.images[i].tex.setSmooth(true);
            ld.images[i].sprite = sf::Sprite(ld.images[i].tex);
            ld.images[i].sprite->setOrigin({ ld.images[i].tex.getSize().x / 2.f, ld.images[i].tex.getSize().y / 2.f });
            ld.images[i].sprite->setPosition({ imgs[i].x, imgs[i].y });
            ld.images[i].sprite->setScale({ imgs[i].sc, imgs[i].sc });
        }

        ld.textCount = 7;
        struct TxtInfo { const char* str; int sz; Color col; float x, y; bool c; };
        TxtInfo txts[7] = {
            {"3. Level 3",                                    26, Color(255,220,50),  415.f, 55.f,  true},
            {"Now with even more Frenzy!",                    16, Color(255,255,255), 415.f, 88.f,  true},
            {"Frenzy Time!",                                  17, Color(240,240,240), 150.f, 145.f, false},
            {"Now that you're warmed up, it's time to get your Frenzy on.\nEat fish quickly to fill up the FRENZY METER. Each time you\nfill the meter, your Frenzy Multiplier increases and everything\nyou eat is worth more points! Try to reach Mega Frenzy for a 6x bonus!",
                                                              14, Color(240,240,240), 150.f, 175.f, false},
            {"Frenzy Boost",                                  17, Color(240,240,240), 320.f, 320.f, false},
            {"Look out for special Frenzy Boost power-ups.\nEat a Frenzy Booster to fill up the Frenzy\nMeter in a single chomp!",
                                                              14, Color(240,240,240), 320.f, 345.f, false},
            {"You have to keep eating to keep the Frenzy Meter full!", 15, Color(240,240,240), 390.f, 450.f, true},
        };
        for (int i = 0; i < 7; i++)
        {
            ld.texts[i].text.emplace(ld.font);
            ld.texts[i].text->setString(txts[i].str);
            ld.texts[i].text->setCharacterSize(txts[i].sz);
            ld.texts[i].text->setFillColor(txts[i].col);
            ld.texts[i].text->setOutlineColor(Color(20, 20, 20));
            ld.texts[i].text->setOutlineThickness(i == 0 ? 2.f : 1.f);
            if (txts[i].c) { FloatRect b = ld.texts[i].text->getLocalBounds(); ld.texts[i].text->setOrigin({ b.size.x / 2.f, b.size.y / 2.f }); }
            ld.texts[i].text->setPosition({ txts[i].x, txts[i].y });
        }
    }
}

// button hover states each frame
void UpdateGameScreen()
{
    Vector2f mousePos = window.mapPixelToCoords(Mouse::getPosition(window), view);

    // options & quit hover
    for (int i = 0; i < 2; i++)
    {
        if (!gsBtns[i].sprite) continue;
        if (gsBtns[i].sprite->getGlobalBounds().contains(mousePos))
        {
            gsBtns[i].sprite->setTexture(gsBtns[i].hoverTex);
            gsBtns[i].sprite->setOrigin({ gsBtns[i].hoverTex.getSize().x / 2.f, gsBtns[i].hoverTex.getSize().y / 2.f });
        }
        else
        {
            gsBtns[i].sprite->setTexture(gsBtns[i].normalTex);
            gsBtns[i].sprite->setOrigin({ gsBtns[i].normalTex.getSize().x / 2.f, gsBtns[i].normalTex.getSize().y / 2.f });
        }
    }

    // continue hover (uses custom half-bounds)
    if (gsBtns[2].sprite)
    {
        bool hovered = mousePos.x >= gsBtns[2].x - gsBtns[2].hoverHalfW &&
            mousePos.x <= gsBtns[2].x + gsBtns[2].hoverHalfW &&
            mousePos.y >= gsBtns[2].y - gsBtns[2].hoverHalfH &&
            mousePos.y <= gsBtns[2].y + gsBtns[2].hoverHalfH;
        if (hovered)
        {
            gsBtns[2].sprite->setTexture(gsBtns[2].hoverTex);
            gsBtns[2].sprite->setOrigin({ gsBtns[2].hoverTex.getSize().x / 2.f, gsBtns[2].hoverTex.getSize().y / 2.f });
            gsBtns[2].sprite->setScale({ gsBtns[2].hoverScale, gsBtns[2].hoverScale });
        }
        else
        {
            gsBtns[2].sprite->setTexture(gsBtns[2].normalTex);
            gsBtns[2].sprite->setOrigin({ gsBtns[2].normalTex.getSize().x / 2.f, gsBtns[2].normalTex.getSize().y / 2.f });
            gsBtns[2].sprite->setScale({ gsBtns[2].scale, gsBtns[2].scale });
        }
    }

    if (!showQuitPopup) return;

    // quit popup button hover
    for (int i = 0; i < 3; i++)
    {
        auto& btn = quitPopup.btns[i];
        if (!btn.sprite) continue;
        if (btn.sprite->getGlobalBounds().contains(mousePos))
        {
            btn.sprite->setTexture(btn.hoverTex);
            btn.sprite->setOrigin({ btn.hoverTex.getSize().x / 2.f, btn.hoverTex.getSize().y / 2.f });
        }
        else
        {
            btn.sprite->setTexture(btn.normalTex);
            btn.sprite->setOrigin({ btn.normalTex.getSize().x / 2.f, btn.normalTex.getSize().y / 2.f });
        }
    }
}

// draw buttons + quit popup OR level images/texts
void DrawGameScreen()
{
    window.setView(view);
    window.clear(Color::Black);
    window.draw(gameScreenBgSprite);

    for (int i = 0; i < 3; i++)
        if (gsBtns[i].sprite) window.draw(*gsBtns[i].sprite);

    if (showQuitPopup)
    {
        if (quitPopup.bgSprite)    window.draw(*quitPopup.bgSprite);
        if (quitPopup.titleSprite) window.draw(*quitPopup.titleSprite);
        for (int i = 0; i < 3; i++)
            if (quitPopup.btns[i].sprite) window.draw(*quitPopup.btns[i].sprite);
        window.draw(quitPopupLine1);
        window.draw(quitPopupLine2);
    }
    else
    {
        LevelData& ld = levels[selectedLevel - 1];
        for (int i = 0; i < ld.imageCount; i++)
            if (ld.images[i].sprite) window.draw(*ld.images[i].sprite);
        for (int i = 0; i < ld.textCount; i++)
            if (ld.texts[i].text) window.draw(*ld.texts[i].text);
    }

    window.display();
}

// main game screen loop
void GameScreen(int level)
{
    StartGameScreen(level);
    Clock clock;
    while (window.isOpen())
    {
        float dt = clock.restart().asSeconds();
        totaltime += dt;
        while (const optional event = window.pollEvent())
        {
            if (event->is<Event::Closed>()) window.close();
            else if (const auto* resizeEvent = event->getIf<Event::Resized>())
            {
                view.setViewport(FloatRect({ 0.f, 0.f }, { 1.f, 1.f }));
                window.setView(view);
            }

            // ESC → exit
            if (const auto* keyPressed = event->getIf<Event::KeyPressed>())
                if (keyPressed->code == Keyboard::Key::Escape) return;

            // left click → buttons / quit popup
            if (const auto* mouseBtn = event->getIf<Event::MouseButtonReleased>())
            {
                if (mouseBtn->button == Mouse::Button::Left)
                {
                    Vector2f mousePos = window.mapPixelToCoords(Mouse::getPosition(window), view);
                    if (showQuitPopup)
                    {
                        if (quitPopup.btns[0].sprite && quitPopup.btns[0].sprite->getGlobalBounds().contains(mousePos))
                        {
                            showQuitPopup = false; MainMenu(); return;
                        }
                        if (quitPopup.btns[1].sprite && quitPopup.btns[1].sprite->getGlobalBounds().contains(mousePos))
                        {
                            showQuitPopup = false; MainMenu(); return;
                        }
                        if (quitPopup.btns[2].sprite && quitPopup.btns[2].sprite->getGlobalBounds().contains(mousePos))
                            showQuitPopup = false;
                    }
                    else
                    {
                        if (gsBtns[0].sprite && gsBtns[0].sprite->getGlobalBounds().contains(mousePos)) OptionsMenu();
                        if (gsBtns[1].sprite && gsBtns[1].sprite->getGlobalBounds().contains(mousePos)) showQuitPopup = true;
                    }
                }
            }
        }
        UpdateGameScreen();
        DrawGameScreen();
    }
}