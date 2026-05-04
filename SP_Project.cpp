#include <SFML/Graphics.hpp>
#include <SFML/Main.hpp>
#include <SFML/Network.hpp>
#include <SFML/System.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Audio/SoundBuffer.hpp>
#include <SFML/Audio/Sound.hpp>
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
#include <cstring>
#include <random>
#include <optional>
#include <cstdint>
#include <fstream>
#include <iomanip>
#include <sstream>
using namespace std;
using namespace sf;

struct MenuFish
{
    Sprite *sprite = nullptr;
    float *velocityX = nullptr;
    float *velocityY = nullptr;
    float *changedir = nullptr;
    bool turning = false;
    int turnFrame = 0;
};
MenuFish mGreen, mMinow, mCuda, mQueen;

void LoadingScreen();
void StartLoadingScreen();
bool DrawLoadingScreen(float totalTime);
void MainMenu();
void StartMainMenu();
void UpdateMainMenu();
void DrawMainMenu();
void MainMenuFishAnimation();
void GreenfishAnimation(MenuFish &fish);
void MinowFishanimation(MenuFish &fish);
void BarracudaFishanimation(MenuFish &fish);
void QueenTriggerFish(MenuFish &fish);
void ChangingButtonShape();
void SwitchUser();
void ResetStats();
void StartSwitchUser();
void CreateButton(Sprite &sprite, Texture &texture, const string &filePath,
                  float xPosition,
                  float yPositon, float scaleX, float scaleY);
void SetupButtonText(Text &text, const string &str, Sprite &button);
void UpdateSwitchUser();
void HoverButton(Sprite &button, const Texture &normalTex,
                 const Texture &highlightTex, Vector2f mousePos, Text &text);
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
void centerText(Text &text);
void bglevel();
void Startbglevel();
void Drawbglevel();
void Movingplayer();
void Startmovingplayer();
void Updatemovingplayer(float deltaTime = 0.f);
void Drawmovingplayer();
void StartSmallFish();
void UpdateSmallFishes(float dt);
void DrawSmallFishes(RenderWindow &window);
void StartMediumFish();
void UpdateMediumFishes(float dt);
void DrawMediumFishes(sf::RenderWindow &window);
void StartLargeFish();
void UpdateLargeFishes(float dt);
void DrawLargeFishes(sf::RenderWindow &window);
void StartGameBubble(float x, float y, bool isAction);
void UpdateGameBubbles(float dt);
void DrawGameBubbles();
void createScorePopup(float x, float y, int points);
void createScorePopup(float x, float y, String power);
void StartMermaidEvent();
void UpdateMermaidEvent(float dt);
void DrawMermaidEvent();
void Timeattacklevel();
void LevelHud();
void StartLevelHud();
void UpdateLevelHud();
void DrawLevelHud(bool doClear = true);
void ShowPauseMenu();
bool ShowEndGameMenu();
void LoadGameData();
void SaveGameData();
void QuitLevelLoadingScreen();
void StartPowerUps();
void UpdatePowerUps(float dt);
void DrawPowerUps();
void loadSounds();
void StartEndLevel(RenderWindow& window);
void UpdateEndLevel(RenderWindow& window);
void DrawEndLevel(RenderWindow& window);
bool EndLevel();

float getRandom(float min, float max)
{
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dist(min, max);
    return dist(gen);
}

float WindowWidth = 800;
float WindowHeight = 600;
RenderWindow window(VideoMode({(unsigned int)WindowWidth, (unsigned int)WindowHeight}), "Feeding Frenzy 2");
View view({400.f, 300.f}, Vector2f(window.getSize()));
View uiView({400.f, 300.f}, {800.f, 600.f});

const int SOUND_COUNT = 5;
SoundBuffer soundBuffers[SOUND_COUNT];
Sound *sounds[SOUND_COUNT];
bool isgamescreenopen = false;
// =================================== Loading Screen =================================
Shader swayShader;
Shader swayShaderPlants;

float deltaTime;
float startX = WindowWidth / 2.f;
float spacing = 65.0f;
float width_bubble_global;
float endTimer = 0.0f;
bool transitionStarted = false;
Vector2f ventPositions[2] = {{WindowWidth - 140.f, WindowHeight - 157.5f}, {WindowWidth - 220.f, WindowHeight - 117.f}};
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
Texture bgTexture("Assets/Select_level/gamemap_bg.jpeg");
Sprite bgSprite(bgTexture);
Texture signTexture("Assets/Select_level/gamemap_tittleboard.jpeg");
Sprite signSprite(signTexture);
Texture bannerTexture("Assets/Select_level/bord.jpeg");
Sprite bannerSprite(bannerTexture);

// back button (normal / hover)
Texture myTexture("Assets/Select_level/shell_tinybtn124_normal.jpeg");
Texture myHoverTexture("Assets/Select_level/shell_tinybtn124_high.jpeg");
Sprite mySprite(myTexture);

// pearl textures (locked / unlocked) + load bar
Texture pearlTexture("Assets/Select_level/white.jpeg");
Texture pearlUnlockedTexture("Assets/Select_level/gold.jpeg");
Texture loadBarTexture("Assets/Select_level/gamemap_loadbar.jpeg");

// sound of loading
Music loadingmusic("Assets/Music and Sounds/01_loadTrack.mp3");

// pearl sprites - normal & time attack
Sprite pearlSprite1(pearlTexture), pearlSprite2(pearlTexture), pearlSprite3(pearlTexture);
Sprite ta_pearlSprite1(pearlTexture), ta_pearlSprite2(pearlTexture), ta_pearlSprite3(pearlTexture);

// pearl positions & scales (locked / unlocked)
sf::Vector2f lockedPos1 = {220.f, 87.f};
sf::Vector2f lockedScale1 = {0.08f, 0.08f};
sf::Vector2f lockedPos2 = {249.f, 133.f};
sf::Vector2f lockedScale2 = {0.06f, 0.06f};
sf::Vector2f lockedPos3 = {246.f, 170.f};
sf::Vector2f lockedScale3 = {0.06f, 0.06f};

sf::Vector2f unlockedPos1 = {235.f, 110.f};
sf::Vector2f unlockedScale1 = {0.036f, 0.036f};
sf::Vector2f unlockedPos2 = {261.f, 152.f};
sf::Vector2f unlockedScale2 = {0.029f, 0.029f};
sf::Vector2f unlockedPos3 = {255.f, 186.f};
sf::Vector2f unlockedScale3 = {0.028f, 0.028f};

// UI texts
static sf::Font font("Assets/Fonts/ARIALNBI.ttf");
static sf::Text staticTxt(font, ""), nowLoadingTxt(font, ""), menuTxt(font, ""), levelTxt(font, ""), loadingtxt(font, "");

// level unlock state
bool level1Unlocked = true, level2Unlocked = false, level3Unlocked = false;
bool ta_level1Unlocked = true, ta_level2Unlocked = false, ta_level3Unlocked = false;
bool isTimeAttackMode = false;
bool loadingdone = false;

// pearl data struct
struct PearlData
{
    sf::Sprite *sprite;
    float radius;
    std::string name;
    bool *unlocked;
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
Texture normal_options_bott, hover_options_bott;
optional<Sprite> gsBtn1Sprite;
const float GS_BTN1_X = 180.f, GS_BTN1_Y = 560.f, GS_BTN1_SCALE = 1 / 10.0f;

Texture normal_quit_bott, hover_quit_bott;
optional<Sprite> gsBtn2Sprite;
const float GS_BTN2_X = 630.f, GS_BTN2_Y = 560.f, GS_BTN2_SCALE = 2 / 10.0f;

Texture normal_continue_bott, hover_continue_bott;
optional<Sprite> gsBtn3Sprite;
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
struct QuitPopupButton
{
    Texture normalTex, hoverTex;
    optional<Sprite> sprite;
};
struct QuitPopup
{
    Texture bgTex, titleTex;
    optional<Sprite> bgSprite, titleSprite;
    QuitPopupButton btns[3];
};
QuitPopup quitPopup;

// level assets structs
struct LevelImage
{
    Texture tex;
    optional<Sprite> sprite;
    float x, y, scale;
};
struct LevelText
{
    optional<Text> text;
    string str;
    int size;
    Color color;
    float x, y;
    bool centered;
};
struct LevelData
{
    Font font;
    LevelImage images[4];
    int imageCount;
    LevelText texts[7];
    int textCount;
};
LevelData levels[3];

// quit popup texts & layout constants
Text quitPopupLine1(font, "", 22), quitPopupLine2(font, "", 18);
const float QUIT_POPUP_X = 400.f, QUIT_POPUP_Y = 330.f;
const float QUIT_BTN1_Y = 300.f, QUIT_BTN2_Y = 360.f, QUIT_BTN3_Y = 420.f;
const float QUIT_BTN_SCALE = 0.7f;
bool pearlClicked = false;

Texture Background_Loading("Assets/Main menu & Loading/Loading Screen/Images/loadbg.png");
Sprite Background_Loading_sprite(Background_Loading);
Texture Game_Icon("Assets/Main menu & Loading/Loading Screen/Images/LOGO.png");
Sprite Game_Icon_sprite(Game_Icon);
Texture POPCAP_Logo("Assets/Main menu & Loading/Loading Screen/Images/popCapLogo.png");
Sprite POPCAP_Logo_sprite(POPCAP_Logo);
Texture LogoWood("Assets/Main menu & Loading/Loading Screen/Images/sproutLogoWood.png");
Sprite LogoWood_sprite(LogoWood);
Texture loadBar("Assets/Main menu & Loading/Loading Screen/Images/loadBar.png");
Sprite loadBar_sprite(loadBar);
Texture loadBar_empty("Assets/Main menu & Loading/Loading Screen/Images/loadBarCap.png");
Sprite loadBar_empty_sprite(loadBar_empty);
Texture bubbleTexture("Assets/Main menu & Loading/Loading Screen/Images/bubbles_letters.png");
Sprite bubbleSprite(bubbleTexture);
Texture fontTexture("Assets/Fonts/LOADING.png");
Sprite s_L(fontTexture);
Sprite s_O(fontTexture);
Sprite s_A(fontTexture);
Sprite s_D(fontTexture);
Sprite s_I(fontTexture);
Sprite s_N(fontTexture);
Sprite s_G(fontTexture);
Texture bubbleSmallTex("Assets/Main menu & Loading/Loading Screen/Images/fx_bubble_d0.png");
Sprite bubbleSmallSprite(bubbleSmallTex);
Texture spark("Assets/Main menu & Loading/Loading Screen/Images/sparkleb0.png");
Sprite sparkSprite(spark);
SoundBuffer BubblesdoneBuffer("Assets/Music and Sounds/bubbleexploded.mpeg");
Sound Bubbledone(BubblesdoneBuffer);

// ===============================================================================
// =================================== Main Menu =================================
// ===============================================================================
Texture mainbackground("Assets/Main menu & Loading/Main Menu/mm_mainmenubg.jpg");
Sprite background(mainbackground);
Texture logo("Assets/Main menu & Loading/Main Menu/logo-removebg-preview.png");
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

// Text for welcome user
Font mainFont("Assets/Fonts/ARIALNBI.TTF");
Text welcomeLabel(mainFont, "", 0);
Text userNameText(mainFont, "", 0);
string userName = "Mohammad Sayed";

// Welcome Button
Texture welcometex("Assets/Main menu & Loading/Main Menu/Welcome.png");
Sprite welcomebutton(welcometex);

Texture Barracudatex("Assets/Fish/Barracuda/Barracuda.png");
Sprite Barracuda(Barracudatex);
int BFcol = 0;
int BFrow = 0;
float BFvelocityX_AXIS = -2;
float BFvelocityY_AXIS = -2;
float BFchangedir = 1;

Texture startgametex("Assets/Main menu & Loading/Main Menu/mm_startgame_normal-removebg-preview.png");
Texture startgamepressed("Assets/Main menu & Loading/Main Menu/mm_startgame_high-removebg-preview.png");
Sprite startgamebutton(startgametex);

SoundBuffer buttonpressed("Assets/Music and Sounds/mouseover.ogg");
Sound buttonpressedsound(buttonpressed);

Music mainmenumusic("Assets/Music and Sounds/menuMusic.mp3");
bool isMusicEnabled = true;

Texture timeattacktex("Assets/Main menu & Loading/Main Menu/mm_timeattack_normal-removebg-preview.png");
Texture timeattackpressed("Assets/Main menu & Loading/Main Menu/mm_timeattack_high-removebg-preview.png");
Sprite timeattackbutton(timeattacktex);

Texture highscoretex("Assets/Main menu & Loading/Main Menu/mm_highscores_normal-removebg-preview.png");
Texture highscorepressed("Assets/Main menu & Loading/Main Menu/mm_highscores_high-removebg-preview.png");
Sprite highscorebutton(highscoretex);

Texture quittex("Assets/Main menu & Loading/Main Menu/mm_quit_normal-removebg-preview.png");
Texture quitpressed("Assets/Main menu & Loading/Main Menu/mm_quit_high-removebg-preview.png");
Sprite quitbutton(quittex);

Texture optionstex("Assets/Main menu & Loading/Main Menu/mm_options_normal-removebg-preview.png");
Texture optionspressed("Assets/Main menu & Loading/Main Menu/mm_options_high-removebg-preview.png");
Sprite optionsbutton(optionstex);

Texture switchusertex("Assets/Main menu & Loading/Main Menu/mm_switchuser_normal-removebg-preview.png");
Texture switchuserpressed("Assets/Main menu & Loading/Main Menu/mm_switchuser_high-removebg-preview.png");
Sprite switchuserbutton(switchusertex);

Texture creditstex("Assets/Main menu & Loading/Main Menu/mm_credits_normal-removebg-preview.png");
Texture creditspressed("Assets/Main menu & Loading/Main Menu/mm_credits_high-removebg-preview.png");
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
    void update(int frameWidth, int frameHeight, float dt)
    {
        sprite.move({velocityX_AXIS * dt * 60.f, velocityY_AXIS * dt * 60.f});
        changedir = (velocityX_AXIS > 0) ? -1.f : 1.f;
        sprite.setScale({0.2f * changedir, 0.2f});
        timer = 0.0f;
        sprite.setTextureRect(IntRect({currentFrame * frameWidth, 0}, {frameWidth, frameHeight}));
        currentFrame++;
        if (currentFrame == 15)
        {
            currentFrame = 0;
        }
    }
};
vector<AnimatedObject> smallfishs;

RectangleShape Barracudacollieder(Vector2f{271, 121});
RectangleShape QueenTriggercollieder(Vector2f{260, 160});
int cu = 0;
float totaltime = 0.0f;
Vector2i mouseLocalPos;
Vector2f mouseWorldPos;
CircleShape c({10});

Texture QueenTrigerTEX("Assets/Fish/QueenTrigger/QueenTrigger.png");
Sprite QueenTrigger(QueenTrigerTEX);
int QTcol = 0;
int QTrow = 0;
float QTchangedir = 1;
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
    long long score = 0;
    // ---> INDIVIDUAL PROGRESS <---
    bool level1Unlocked = true, level2Unlocked = false, level3Unlocked = false;
    bool ta_level1Unlocked = true, ta_level2Unlocked = false, ta_level3Unlocked = false;
};
Player players[7];
int NumberOfUsers = 0;
const string SAVE_FILE = "Assets/Switch User/save.txt";
const int MaxNumberOfUsers = 7;
int SelectedUser = -1;
Text *PlayersTexts[8];
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

Texture quit_bg("Assets/Quit/bg_quit.png");
Sprite quit_bg_sprite(quit_bg);
Texture quit_button("Assets/Quit/Button.png");
Sprite quit_button_sprite(quit_button);
Texture quit_button_hover("Assets/Quit/Button High.png");
Sprite quit_button_hover_sprite(quit_button_hover);
Texture quit_icon("Assets/Quit/quitgame_icon.png");
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
Font barmenoBoldFont;
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
bool isSoundEnabled = true;
bool isFullscreen = true;

float barX = 258.f;
float barY = 495.f;
float barWidth = 308.f;
float barHeight = 28.f;
float barSpeed = 150.f;
bool isLoading = false;
float loadProgress = 0.f;

Texture texHudTop, texHudBottom, texHudGrowth, texHudGrowthMarker;
Sprite sprHudTop(texHudTop), sprHudBottom(texHudBottom), sprHudGrowth(texHudGrowth);
Sprite sprHudGrowthMarker1(texHudGrowthMarker), sprHudGrowthMarker2(texHudGrowthMarker);
Texture texHudFrenzy;
Font frenzyFont;
optional<Text> txtFrenzyLetters[7]; 
int frenzyProgress = 0;
bool isLevelRunning = false;
float timeAttackTimer = 0.f;
float timeAttackLimit = 90.f;      
float frenzyDecayAccumulator = 0.f; 

Texture texTimerBg;
Sprite sprTimerBg(texTimerBg);
Font timerFont;
Text txtTimer(timerFont, "", 22);
struct FishIcon
{
    optional<Texture> normalTex;
    optional<Texture> shadowTex;
    optional<Sprite> sprite;
    bool canEat = false;
};

FishIcon fishIcons[3]; // herring, cod, lionfish
Texture dangerIconTex;
Sprite dangerIcon(dangerIconTex);
int playerSize = 1;
Font multiplierFont;
Text txtMultiplier(multiplierFont, "", 35);
Text txtScore(multiplierFont, "", 40);

RectangleShape growthBarFill;
float growthPercentage = 0.f; // 0.0 to 100.0

Texture texHudAbility;
Sprite sprHudAbility(texHudAbility);

// --- Level Quit Loading Screen ---
Texture texLevelLoadingBg, texLevelLoadingLogo;
Sprite sprLevelLoadingBg(texLevelLoadingBg), sprLevelLoadingLogo(texLevelLoadingLogo);
Texture texLevelLoadBar, texLevelLoadBarCap;
Sprite sprLevelLoadBar(texLevelLoadBar), sprLevelLoadBarCap(texLevelLoadBarCap);
float levelLoadingProgress = 0.f;
Font QuitLevelFont;
Text txtQuitLevel(QuitLevelFont, "Loading...", 28);
bool goToMainMenuFromLevel = false;
bool g_optionsFromPause = false;
bool g_inEndScreen = false;

RectangleShape fadeRect;
Clock fadeClock;
bool isFadingOut = false;
bool isFadingIn = false;
float fadeAlpha = 0.f;
int storyCarryScore = 0;
int taCarryScore = 0;
int lastWonStoryLevel = 0;
int lastWonTALevel = 0;

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
Texture texHSDoneNormal("Assets/Highscore/done_normal.png");
Texture texHSDoneHover("Assets/Highscore/done_hover.png");
Sprite sprHSDonePlank(texHSDoneNormal);

Texture texHSResetNormal("Assets/Highscore/shell_tinybtn124_normal.png");
Texture texHSResetHover("Assets/Highscore/shell_tinybtn124_high.jpg");
Sprite sprHSResetPlank(texHSResetNormal);

Texture texHSArrowNormal("Assets/Highscore/_sidescrollbtn.png");
Texture texHSArrowHover("Assets/Highscore/_sidescrollbtnhi.png");
Texture texHSArrowDown("Assets/Highscore/_sidescrollbtndown.png");
Sprite sprHSStoryArrowRight(texHSArrowNormal);
Sprite sprHSStoryArrowLeft(texHSArrowNormal);
Sprite sprHSListArrowUp(texHSArrowNormal);
Sprite sprHSListArrowDown(texHSArrowNormal);

Font fontHSTitle("Assets/Fonts/BernardMT.ttf");
Font fontHSMain("Assets/Fonts/Barmeno.ttf");
Font fontHSDone("Assets/Fonts/BarmenoBold.otf");

Text textHSMainTitle(fontHSTitle, "High Scores", 48);
Text textHSStoryMode(fontHSMain, "Story Mode", 22);
Text textHSDoneButton(fontHSDone, "Done", 30);
Text textHSResetButton(fontHSMain, "Reset All Scores", 16);

Text textHSListRanks[VISIBLE_SCORES] = {
    {fontHSMain, "", 20}, {fontHSMain, "", 20}, {fontHSMain, "", 20}, {fontHSMain, "", 20}, {fontHSMain, "", 20}, {fontHSMain, "", 20}, {fontHSMain, "", 20}, {fontHSMain, "", 20}, {fontHSMain, "", 20}, {fontHSMain, "", 20}};
Text textHSListNames[VISIBLE_SCORES] = {
    {fontHSMain, "", 20}, {fontHSMain, "", 20}, {fontHSMain, "", 20}, {fontHSMain, "", 20}, {fontHSMain, "", 20}, {fontHSMain, "", 20}, {fontHSMain, "", 20}, {fontHSMain, "", 20}, {fontHSMain, "", 20}, {fontHSMain, "", 20}};
Text textHSListScores[VISIBLE_SCORES] = {
    {fontHSMain, "", 20}, {fontHSMain, "", 20}, {fontHSMain, "", 20}, {fontHSMain, "", 20}, {fontHSMain, "", 20}, {fontHSMain, "", 20}, {fontHSMain, "", 20}, {fontHSMain, "", 20}, {fontHSMain, "", 20}, {fontHSMain, "", 20}};

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
Font fontcredits("Assets/Fonts/trebuc.ttf");
optional<Text> creditsText;
Text textHSDoneButtoncredits(fontHSDone, "Done", 30);
Texture texHSDoneNormalcredits("Assets/Credits/done_normal.png");
Texture texHSDoneHovercredits("Assets/Credits/done_hover.png");
Sprite sprHSDonePlankcredits(texHSDoneNormalcredits);

// bg level
Texture texbglevel("Assets/Levels/dribblecastle_backgrounD.png");
Sprite sprbglevel(texbglevel);
Texture reefsledge("Assets/Levels/dribblecastle_ledge_01.jpg");
Sprite sprreefsledge(reefsledge);
Texture reefsledge2("Assets/Levels/dribblecastle_ledge_02.jpg");
Sprite sprreefsledge2(reefsledge2);
Texture reefsoverhang("Assets/Levels/dribblecastle_overhang_01.jpg");
Sprite sprreefsoverhang(reefsoverhang);
Texture reefsgrass("Assets/Levels/sharedplants_grass_01.jpg");
Sprite sprreefsgrass(reefsgrass);
Texture reefsplants("Assets/Levels/sharedplants_plant_01.jpg");
Sprite sprreefsplants(reefsplants);
Texture reefsplants2("Assets/Levels/sharedplants_plant_02.jpg");
Sprite sprreefsplants2(reefsplants2);
Texture reefsplants23("Assets/Levels/sharedplants_plant_023.jpg");
Sprite sprreefsplants23(reefsplants23);
Texture reefsseaweed("Assets/Levels/sharedplants_seaweed_01.jpg");
Sprite sprreefsseaweed(reefsseaweed);
Texture reefsseaweed22("Assets/Levels/sharedplants_seaweed_01.jpg");
Sprite sprreefsseaweed22(reefsseaweed22);
Texture reefsseaweed2("Assets/Levels/sharedplants_seaweed_02.jpg");
Sprite sprreefsseaweed2(reefsseaweed2);
Texture reefsseaweed3("Assets/Levels/sharedplants_seaweed_03.jpg");
Sprite sprreefsseaweed3(reefsseaweed3);
Texture reefsseaweed31("Assets/Levels/sharedplants_seaweed_03.jpg");
Sprite sprreefsseaweed31(reefsseaweed31);
Texture reefsseaweed32("Assets/Levels/sharedplants_seaweed_03.jpg");
Sprite sprreefsseaweed32(reefsseaweed32);
Texture reefsseaweed33("Assets/Levels/sharedplants_seaweed_03.jpg");
Sprite sprreefsseaweed33(reefsseaweed33);
Texture reefsseaweed4("Assets/Levels/sharedplants_seaweed_04.jpg");
Sprite sprreefsseaweed4(reefsseaweed4);
Texture reefstubes("Assets/Levels/sharedplants_tubes_01.jpg");
Sprite sprreefstubes(reefstubes);
Texture reefstubes2("Assets/Levels/sharedplants_tubes_02.jpg");
Sprite sprreefstubes2(reefstubes2);
Texture reefstubes3("Assets/Levels/sharedplants_tubes_03.jpg");
Sprite sprreefstubes3(reefstubes3);

// player
Texture texPlayerSwim("Assets/Fish/butterflyfish/swim.png");
Texture texPlayerEat("Assets/Fish/butterflyfish/eat.png");
Texture texPlayerTurn("Assets/Fish/butterflyfish/turn.png");
Texture texPlayerIdle("Assets/Fish/butterflyfish/idle.png");
Texture texPlayerall("Assets/Fish/butterflyfish/Butterflyfishall.png");
Sprite sprPlayerSwim(texPlayerSwim);
Sprite sprPlayerEat(texPlayerEat);
Sprite sprPlayerTurn(texPlayerTurn);
Sprite sprPlayerIdle(texPlayerIdle);
Sprite sprPlayerall(texPlayerall);

// ============== Power-Ups =========================

// Power-up type IDs
// 0 = Time Bonus (+5 sec, time attack only)
// 1 = Star       (+100 * multiplier score)
// 2 = Speed Boost (5 sec speed x2)

struct PowerUp
{
    float x, y; // world position
    float vy;   // falling speed (pixels/sec)
    int type;   // 0 / 1 / 2
    bool active;

    bool isPopping; // true once the player touches it
    float popTimer; // counts up while popping
    float scaleX;   // current render scale
    float scaleY;
};

const int MAX_POWERUPS = 6;
PowerUp powerUps[MAX_POWERUPS];

float powerUpSpawnTimer = 0.f;
float powerUpSpawnInterval = 15.f; // one bubble every 8 seconds

bool speedBoostActive = false;
float speedBoostTimer = 0.f;
const float SPEED_BOOST_DURATION = 5.f;
const float SPEED_BOOST_MULT = 2.0f;

Texture texPowerUpTime("Assets\\bouns\\timebubble1.png");
Texture texPowerUpStar("Assets\\bouns\\starbubble1.png");
Texture texPowerUpSpeed("Assets\\bouns\\speedbubble1.png");
Texture texPowerUpShrink("Assets\\bouns\\shrinkbubble1.png");
Texture texBubblePop0("Assets/bouns/_bubblepop0.png");
Texture texBubblePop1("Assets/bouns/_bubblepop1.png");
bool powerUpTexLoaded = false;

// ==========================================
// 1.       (Global Variables)
// ==========================================


enum PlayerState
{
    EAT = 0,
    IDLE = 1,
    SWIM = 2,
    TURN = 3
};


const int frameCounts[4] = {6, 12, 14, 8};


PlayerState currentState = IDLE;
int currentFrame = 0;
float timer = 0.f;

int playerIntroStep = 0;
bool pendingEat = false; 


const int GRID_W = 288;
const int GRID_H = 224; 

const int DRAW_W = 288;
const int DRAW_H = 224;


const float FISH_SCALE = 0.275f;


float lastMouseX = 0.0f;
bool isFacingRight = false;


sf::Vector2i lastScreenMousePos = {0, 0};
sf::Vector2f targetPos = {400.f, 300.f};


float LevelWidth = 950.0f;
float LevelHeight = 750.0f;

bool playerIntroActive = true;
float playerIntroY = -150.f;

// fish
const int MAX_SMALL_FISH = 10;

sf::Texture smallFishTexture("Assets/Fish/herfish/herFish.png");
float spawnTimer = 0.f;
float spawnInterval = 0.5f;

struct SmallFish
{
    sf::Sprite *sprite;
    sf::Vector2f velocity;
    float originalSpeedX;
    bool active;

    bool isFleeing = false;
    float verticalTimer = 0.f;
    bool canTurn = false;
    bool hasTurned = false;
    bool isTurning = false;
    float turnTimer = 0.f;
    float timeToNextTurn = 0.f;
    int currentFrame = 0;
    float animTimer = 0.f;
    bool facingRight = true;
    float spawnX = 0.f;

    SmallFish()
    {
        sprite = nullptr;
        velocity = {0.f, 0.f};
        active = false;
    }

    ~SmallFish()
    {
        if (sprite != nullptr)
        {
            delete sprite;
            sprite = nullptr;
        }
    }
};
void loadSounds()
{
    const char *paths[SOUND_COUNT] = {
        "Assets/Music and Sounds/DoubleFrenzy/DoubleFrenzy.wav",
        "Assets/Music and Sounds/DoubleFrenzy/TrippleFrenzy.wav",
        "Assets/Music and Sounds/DoubleFrenzy/SuperFrenzy.wav",
        "Assets/Music and Sounds/DoubleFrenzy/MegaFrenzy.wav",
        "Assets/Music and Sounds/DoubleFrenzy/FeedingFrenzy.wav"};

    for (int i = 0; i < SOUND_COUNT; i++)
    {
        if (!soundBuffers[i].loadFromFile(paths[i]))
        {
            std::cout << "Failed to load sound " << i << "\n";
        }

        sounds[i] = new Sound(soundBuffers[i]);
    }
    if (WaterSound.loadFromFile("Assets/Music and Sounds/waterloop1.ogg"))
    { 
        waterloop.emplace(WaterSound);
        waterloop->setLooping(true);
    }
    (void)sfxBuffers[0].loadFromFile("Assets/Music and Sounds/wateramb1.ogg"); 
    (void)sfxBuffers[1].loadFromFile("Assets/Music and Sounds/wateramb2.ogg"); 
    (void)sfxBuffers[2].loadFromFile("Assets/Music and Sounds/wave1.ogg");     
    (void)sfxBuffers[3].loadFromFile("Assets/Music and Sounds/wave2.ogg");    
    nextSfxDelay = (rand() % 2500 + 2500) / 1000.f;

}

SmallFish smallFishes[MAX_SMALL_FISH];

// ==========================================
// Medium Fish (QueenTrigger) Settings
// ==========================================
const int MEDIUM_COLS = 14;
const int MEDIUM_FRAME_W = 283;
const int MEDIUM_FRAME_H = 156;
const int MEDIUM_FRAMES_SWIM = 12;
const int MEDIUM_FRAMES_EAT = 6;
const int MEDIUM_FRAMES_TURN = 5;

const int MAX_MEDIUM_FISH = 5;
sf::Texture mediumFishTexture;

struct MediumFish
{
    sf::Sprite *sprite = nullptr;
    sf::Vector2f velocity;
    bool active = false;

    float timeToNextTurn = 0.f;

    int currentFrame = 0;
    float animTimer = 0.f;
    float verticalTimer = 0.f;
    float turnTargetX = 0.f;

    bool isFleeing = false;
    int state = 1;
    bool canTurn = false;
    bool hasTurned = false;
    float turnTimer = 0.f;
    float spawnX = 0.f;

    bool facingRight = true;
};

MediumFish mediumFishes[MAX_MEDIUM_FISH];
float mediumSpawnTimer = 0.f;
float mediumSpawnInterval = 6.0f;

// ==========================================
// Large Fish (Barracuda) Settings
// ==========================================
const int LARGE_COLS = 14;
const int LARGE_FRAME_W = 180;
const int LARGE_FRAME_H = 160;
const int LARGE_FRAMES_SWIM = 7;
const int LARGE_FRAMES_TURN = 5;
const int LARGE_FRAMES_EAT = 6;
const int MAX_LARGE_FISH = 3;
sf::Texture largeFishTexture;

struct LargeFish
{
    sf::Sprite *sprite = nullptr;
    sf::Vector2f velocity;
    bool active = false;

    bool isFleeing = false;
    int currentFrame = 0;
    float animTimer = 0.f;
    float verticalTimer = 0.f;
    float turnTargetX = 0.f; 
    float timeToNextTurn = 0.f;

    int state = 1;
    bool canTurn = false;
    bool hasTurned = false;
    float turnTimer = 0.f;
    float spawnX = 0.f;

    bool facingRight = true;
};

LargeFish largeFishes[MAX_LARGE_FISH];
float largeSpawnTimer = 0.f;
float largeSpawnInterval = 10.0f;

// algorithm for level
//  ==========================================
//  Player Growth & Stats System
//  ==========================================
int playerLevel = 1;
int score = 0;
int fishEatenCount = 0;
bool isPlayerDead = false;
Clock respawnClock;
bool isEscapeMode = false;


int smallFishEatenCount = 0;
int mediumFishEatenCount = 0;
int largeFishEatenCount = 0;
int starsEatenCount = 0; 

bool stopSpawning = false;


bool isDashing = false;
Clock dashClock;
float dashCooldown = 0.8f; 
float dashDuration = 0.15f; 
float dashSpeedMult = 4.0f; 

// ==========================================
//        Game Bubbles System 
// ==========================================
struct GameBubble
{
    float x, y, vx, vy;
    float alpha;
    bool active;
    bool isAction; 
};
const int MAX_GAME_BUBBLES = 20;
GameBubble gameBubbles[MAX_GAME_BUBBLES];
float gameBubbleSpawnTimer = 0.f;


int multiplier = 1;


struct ScorePopup
{
    sf::Text text;
    float alpha = 255.f;  
    float ySpeed = -60.f; 
    bool active = false;
    sf::Color baseColor = sf::Color::White; 

   
    ScorePopup() : text(font) {}
};


const int MAX_POPUPS = 20;
ScorePopup scorePopups[MAX_POPUPS];


float noEatTimer = 0.0f;              
const float PROGRESS_PER_EAT = 0.35f; 
const float COMBO_DECAY_TIME = 3.0f;  
const int MAX_MULTIPLIER = 6;        


enum ComboState
{
    FILLING,    
    WAIT_DRAIN, 
    DRAINING,   
    WAIT_DROP   
};

ComboState comboState = FILLING;
float comboTimer = 0.0f;        
float comboProgress = 0.0f;     
const int FISH_TO_LEVEL_UP = 7; 
const float WAIT_TIME = 2.0f;   
const float DRAIN_SPEED = 0.5f; 


int lives = 3;          
int hitsRemaining = 3;   
bool isGameOver = false; 


Sprite s_Sorry_S(fontTexture);
Sprite s_Sorry_O(fontTexture);
Sprite s_Sorry_R(fontTexture);
Sprite s_Sorry_R2(fontTexture);
Sprite s_Sorry_Y(fontTexture);
bool showSorryAnimation = false;
bool sorryExploded = false;
float sorryTimer = 0.f;
float sorryLetterScales[5] = {0.f, 0.f, 0.f, 0.f, 0.f}; 


bool showPerfectAnimation = false;
bool perfectExploded = false;
float perfectTimer = 0.f;
float perfectLetterScales[7] = {0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f}; 


Clock hitCooldownClock;
bool isInvincible = false;


// ==========================================
//          Mermaid Event Variables 
// ==========================================
Texture mermaidTex("Assets/fish/mermaid/mermaid.png");
Sprite mermaidSprite(mermaidTex);
Texture starTexture("Assets/bouns/starbubble1.png");
bool isMermaidEventActive = false;
bool mermaidFinished = false;
Clock eventEndClock;

struct Star
{
    Sprite *sprite = nullptr; 
    Vector2f velocity;
    bool active = false;
};
const int MAX_STARS = 30;
Star stars[MAX_STARS];

// Animation vars
int mermaidFrame = 0;
float mermaidAnimTimer = 0.0f;
float starSpawnTimer = 0.0f;


struct LevelSettings
{
    int fishToGrowToLevel2 = 15; 
    int fishToGrowToLevel3 = 40; 
    int fishToWin = 60;
};


LevelSettings currentLevelSettings;

int gameLEVEL;

enum Gamemode
{
    CLASSIC,
    TIMEATTACK
};
Gamemode currentGamemode = TIMEATTACK;

// Timer variables
float timeAttackDuration = 90.f;
float remainingTime = 0.f;
bool isGameWon = false;
float finalTime = 0.f;
Text timerText(font, "", 40);
bool anyFishLeft;
const sf::Time totalDuration = sf::seconds(90.f);
sf::Clock countdownClock;
bool mermaidStarted = false;
bool hasPlayedExitSound = false;

SoundBuffer eatSoundBuffer("Assets/Music and Sounds/bite1.ogg");
Sound eatSound(eatSoundBuffer);

SoundBuffer eatSoundBuffer2("Assets/Music and Sounds/bite2.ogg");
Sound eatSound2(eatSoundBuffer2);

SoundBuffer PlayergotEatenBuffer("Assets/Music and Sounds/bite3.ogg");
Sound PlayergotEaten(PlayergotEatenBuffer);

Music levelsound("Assets/Music and Sounds/02_track1_gameplay.mp3");

SoundBuffer gameoverBuffer("Assets/Music and Sounds/10_gameOver.mp3");
Sound gameover(gameoverBuffer);

SoundBuffer mermaideventBuffer("Assets/Music and Sounds/11_mermaidSwimLoop.mp3");
Sound mermaidevent(mermaideventBuffer);

SoundBuffer dashBuffer("Assets/Music and Sounds/playerdash.ogg");
Sound dashSound(dashBuffer);

SoundBuffer levelUpBuffer("Assets/Music and Sounds/playergrow.ogg");
Sound levelUpSound(levelUpBuffer);

SoundBuffer StageClearBuffer("Assets/Music and Sounds/08_stageClear.mp3");
Sound StageClearSound(StageClearBuffer);

SoundBuffer spawnBuffer("Assets/Music and Sounds/playerspawn.ogg");
Sound spawnSound(spawnBuffer);

SoundBuffer dieBuffer("Assets/Music and Sounds/09_playerDie.mp3");
Sound dieSound(dieBuffer);

Music WaveSound("Assets/Music and Sounds/waterloop1.ogg");

bool hasPlayedSpawnSound = false;
Clock spawnDelayClock;

struct HighScoreSave
{
    char name[50] = "";
    int score = 0;
};

struct PlayerSave
{
    char name[50] = "";
    int id = 0;
    long long score = 0;
   
    bool level1Unlocked = true, level2Unlocked = false, level3Unlocked = false;
    bool ta_level1Unlocked = true, ta_level2Unlocked = false, ta_level3Unlocked = false;
};

// The Master Save Struct
struct GameSaveData
{
   
    float mouseSpeed = 1.0f;
    int graphicsIndex = 0;
    int soundVolume = 100;
    bool isFullscreen = true;
    bool isMusicEnabled = true;

    
    int numberOfUsers = 0;
    PlayerSave players[7];
    char currentUser[50] = "";

    
    HighScoreSave storyScores[25];
    HighScoreSave timeAttackScores[25];
};


GameSaveData g_data;

void ApplyAudioSettings()
{
    // 1. Set Music Volume (100% or 0%)
    float musicVol = isMusicEnabled ? 100.f : 0.f;
    mainmenumusic.setVolume(musicVol);
    loadingmusic.setVolume(musicVol);
    levelsound.setVolume(musicVol);
    WaveSound.setVolume(musicVol); 

    // 2. Set Sound Volume (100% or 0%)
    float soundVol = isSoundEnabled ? 100.f : 0.f;
    if (waterloop) waterloop->setVolume(soundVol);
    buttonpressedsound.setVolume(soundVol);
    Bubbledone.setVolume(soundVol);
    eatSound.setVolume(soundVol);
    PlayergotEaten.setVolume(soundVol);
    gameover.setVolume(soundVol);
    mermaidevent.setVolume(soundVol);
    dashSound.setVolume(soundVol);
    levelUpSound.setVolume(soundVol);
    StageClearSound.setVolume(soundVol);
    spawnSound.setVolume(soundVol);
    dieSound.setVolume(soundVol);

    // Update frenzy array sounds
    for (int i = 0; i < SOUND_COUNT; i++) {
        if (sounds[i]) sounds[i]->setVolume(soundVol);
    }

 
    for (int i = 0; i < NUM_SOUND_CHANNELS; i++) {
        if (sfxChannels[i]) sfxChannels[i]->setVolume(isSoundEnabled ? 30.f : 0.f);
    }
}
void UpdateAmbientSounds()
{
    // 1. Always keep the base loop playing 
    if (waterloop && waterloop->getStatus() != sf::Sound::Status::Playing) {
        waterloop->play();
    }

    // 2. The Randomizer Logic
    if (sfxTimer.getElapsedTime().asSeconds() >= nextSfxDelay)
    {
        sfxTimer.restart();
        nextSfxDelay = getRandom(2.0f, 6.0f); // Pick a new random delay between 2 and 6 seconds

        // Find an empty audio channel so they can overlap naturally
        for (int i = 0; i < NUM_SOUND_CHANNELS; i++)
        {
            if (!sfxChannels[i] || sfxChannels[i]->getStatus() != sf::Sound::Status::Playing)
            {
                int randomBuffer = rand() % NUM_SFX_BUFFERS; // Pick from 0 to 3
                sfxChannels[i].emplace(sfxBuffers[randomBuffer]);
                sfxChannels[i]->setVolume(isSoundEnabled ? 30.f : 0.f);
                sfxChannels[i]->play();
                break; 
            }
        }
    }
}


void LoadGameData()
{
    ifstream file("savegame.dat", ios::binary);
    if (file.is_open())
    {
        file.read(reinterpret_cast<char *>(&g_data), sizeof(GameSaveData));
        file.close();

        isFullscreen = g_data.isFullscreen;
        isMusicEnabled = g_data.isMusicEnabled;
        isSoundEnabled = (g_data.soundVolume > 0);
        MSpeedIndex = static_cast<int>(g_data.mouseSpeed);
        if (MSpeedIndex < 1 || MSpeedIndex > 4)
            MSpeedIndex = 1;
        GraphicsIndex = g_data.graphicsIndex;
        if (GraphicsIndex < 0 || GraphicsIndex > 2)
            GraphicsIndex = 2;

        NumberOfUsers = g_data.numberOfUsers;
        CurUser = string(g_data.currentUser);
        if (CurUser.empty())
        {
            CurUser = "Guest";
        }

        int activeIndex = 0;
        for (int i = 0; i < 7; i++)
        {
            players[i].name = string(g_data.players[i].name);
            players[i].id = g_data.players[i].id;
            players[i].level1Unlocked = g_data.players[i].level1Unlocked;
            players[i].level2Unlocked = g_data.players[i].level2Unlocked;
            players[i].level3Unlocked = g_data.players[i].level3Unlocked;
            players[i].ta_level1Unlocked = g_data.players[i].ta_level1Unlocked;
            players[i].ta_level2Unlocked = g_data.players[i].ta_level2Unlocked;
            players[i].ta_level3Unlocked = g_data.players[i].ta_level3Unlocked;

            if (players[i].name == CurUser)
            {
                activeIndex = i; // Save the array slot of the current user
            }
        }

       
        level1Unlocked = players[activeIndex].level1Unlocked;
        level2Unlocked = players[activeIndex].level2Unlocked;
        level3Unlocked = players[activeIndex].level3Unlocked;
        ta_level1Unlocked = players[activeIndex].ta_level1Unlocked;
        ta_level2Unlocked = players[activeIndex].ta_level2Unlocked;
        ta_level3Unlocked = players[activeIndex].ta_level3Unlocked;

        for (int i = 0; i < 25; i++)
        {
            story_scores[i].name = string(g_data.storyScores[i].name);
            story_scores[i].score = g_data.storyScores[i].score;
            timeattack_scores[i].name = string(g_data.timeAttackScores[i].name);
            timeattack_scores[i].score = g_data.timeAttackScores[i].score;
        }

        // Safety fallback for empty scores or corrupted first-load logic
        if (story_scores[0].score == 0 || !level1Unlocked || !ta_level1Unlocked)
        {
            level1Unlocked = true;
            ta_level1Unlocked = true;
            for (int i = 0; i < 25; i++)
            {
                story_scores[i] = {"", 0};
                timeattack_scores[i] = {"", 0};
            }
        }
        cout << "Data Loaded Successfully!" << endl;
    }
    else
    {
        cout << "No save file found. Proceeding with defaults." << endl;
        level1Unlocked = true;
        ta_level1Unlocked = true;
        for (int i = 0; i < 25; i++)
        {
            story_scores[i] = {"", 0};
            timeattack_scores[i] = {"", 0};
        }
    }
    ApplyAudioSettings();
}

void SaveGameData()
{
    // 1. Find the current user and save their progress into their specific slot
    for (int i = 0; i < NumberOfUsers; i++)
    {
        if (players[i].name == CurUser)
        {
            players[i].level1Unlocked = level1Unlocked;
            players[i].level2Unlocked = level2Unlocked;
            players[i].level3Unlocked = level3Unlocked;
            players[i].ta_level1Unlocked = ta_level1Unlocked;
            players[i].ta_level2Unlocked = ta_level2Unlocked;
            players[i].ta_level3Unlocked = ta_level3Unlocked;
            break;
        }
    }

    g_data.isFullscreen = isFullscreen;
    g_data.isMusicEnabled = isMusicEnabled;
    g_data.soundVolume = isSoundEnabled ? 100 : 0;
    g_data.mouseSpeed = static_cast<float>(MSpeedIndex);
    g_data.graphicsIndex = GraphicsIndex;

    g_data.numberOfUsers = NumberOfUsers;
    strncpy(g_data.currentUser, CurUser.c_str(), 49);
    g_data.currentUser[49] = '\0';

    for (int i = 0; i < 7; i++)
    {
        strncpy(g_data.players[i].name, players[i].name.c_str(), 49);
        g_data.players[i].name[49] = '\0';
        g_data.players[i].id = players[i].id;
        g_data.players[i].score = players[i].score;
       
        g_data.players[i].level1Unlocked = players[i].level1Unlocked;
        g_data.players[i].level2Unlocked = players[i].level2Unlocked;
        g_data.players[i].level3Unlocked = players[i].level3Unlocked;
        g_data.players[i].ta_level1Unlocked = players[i].ta_level1Unlocked;
        g_data.players[i].ta_level2Unlocked = players[i].ta_level2Unlocked;
        g_data.players[i].ta_level3Unlocked = players[i].ta_level3Unlocked;
    }

    for (int i = 0; i < 25; i++)
    {
        strncpy(g_data.storyScores[i].name, story_scores[i].name.c_str(), 49);
        g_data.storyScores[i].name[49] = '\0';
        g_data.storyScores[i].score = story_scores[i].score;

        strncpy(g_data.timeAttackScores[i].name, timeattack_scores[i].name.c_str(), 49);
        g_data.timeAttackScores[i].name[49] = '\0';
        g_data.timeAttackScores[i].score = timeattack_scores[i].score;
    }

    ofstream file("savegame.dat", ios::binary);
    if (file.is_open())
    {
        file.write(reinterpret_cast<const char *>(&g_data), sizeof(GameSaveData));
        file.close();
        cout << "Data Saved Successfully!" << endl;
    }
    else
    {
        cout << "Error: Could not save data." << endl;
    }
}


// ============================================================
//           END LEVEL - Textures & Sprites
// ============================================================

Texture texEndBg("Assets/EndGameScreen/shell_stageinfo.jpg");
Sprite  sprEndBg(texEndBg);

Texture texOptNormal("Assets/EndGameScreen/options_normal-1 (3).png");
Texture texOptClicked("Assets/EndGameScreen/options_high-1 (2)-Photoroom.png");
Sprite  sprOpt(texOptNormal);

Texture texContNormal("Assets/EndGameScreen/continue_normal-1 (1).png");
Texture texContClicked("Assets/EndGameScreen/continue_high-1 (1).png");
Sprite  sprCont(texContNormal);

Texture texQuitNormal("Assets/EndGameScreen/quit_normal-1 (1).png");
Texture texQuitClicked("Assets/EndGameScreen/quit_high-1 (1).png");
Sprite  sprQuit(texQuitNormal);

Texture texHerring("Assets/EndGameScreen/menu_herring.png");
Sprite  sprHerring(texHerring);

Texture texCod("Assets/EndGameScreen/menu_cod.png");
Sprite  sprCod(texCod);

Texture texLionfish("Assets/EndGameScreen/menu_lionfish.png");
Sprite  sprLionfish(texLionfish);

Texture texStarBubble("Assets/EndGameScreen/starbubble1.png");
Sprite  sprStarBubble(texStarBubble);

Texture texCrunch("Assets/EndGameScreen/chompsplatsmall1.png");
Sprite  sprCrunch(texCrunch);


// ============================================================
//        END LEVEL - Fonts & Score Globals
// ============================================================

Font st_comp("Assets/Fonts/BernardMT.ttf");
Font quit_option_font("Assets/Fonts/BarmenoBold.otf");

int score_1 = 0;
int score_2 = 0;
int score_3 = 0;
int score_4 = 0;

Text score_of_eaten_fish[5] = {
    Text(quit_option_font), Text(quit_option_font),
    Text(quit_option_font), Text(quit_option_font),
    Text(quit_option_font)
};


// ==========================================
//       Player Movement State Globals 
// ==========================================
float currentVisualScale = 0.275f;
bool canDash = true;
bool isDashingNow = false;
bool wasMousePressed = false;
sf::Vector2f dashDirection = { 1.f, 0.f };
float currentRotation = 0.f;
sf::Clock dashTimer;
bool levelWonSuccessfully = false;


int main()
{
    LoadGameData();
    loadSounds();
    cout << "SFML 3.0 and Standard Library are working!" << endl;
    srand(time(0));

    if (isFullscreen)
    {
        window.create(VideoMode::getDesktopMode(), "Feeding Frenzy 2", State::Fullscreen);
    }
    else
    {
        window.create(VideoMode({800, 600}), "Feeding Frenzy 2", State::Windowed);
    }

    LoadingScreen();
    Clock clock;
    Clock totalClock;

    fadeRect.setSize({WindowWidth, WindowHeight});
    fadeRect.setFillColor(Color(0, 0, 0, 0));
    fadeRect.setPosition({0.f, 0.f});

    view.setSize({800.f, 600.f});
    view.setCenter({400.f, 300.f});
    view.setViewport(sf::FloatRect({0.f, 0.f}, {1.f, 1.f}));

    window.setView(view);
    window.setFramerateLimit(120);

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
                view.setViewport(sf::FloatRect({0.f, 0.f}, {1.f, 1.f}));
            }
        }
        window.setView(view);
        window.clear(Color::Black);
        if (DrawLoadingScreen(totalTime))
            break;
    }

    MainMenu();

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
    SaveGameData();
    return 0;
}

void StartLoadingScreen()
{
    if (!swayShader.loadFromFile("sway.frag", Shader::Type::Fragment))
    {
        cout << "Error loading shader!" << endl;
    }
    if (!Background_Loading.loadFromFile("Assets/Main menu & Loading/Loading Screen/Images/loadbg.png"))
    {
        cout << "!!! Error: Could not find loadbg.png !!!" << endl;
    }
    Background_Loading_sprite.setTexture(Background_Loading);
    Background_Loading_sprite.setPosition({0, 0});
    Vector2u texture_Size = Background_Loading.getSize();
    float ScaleX = WindowWidth / texture_Size.x;
    float ScaleY = WindowHeight / texture_Size.y;
    Background_Loading_sprite.setScale({ScaleX, ScaleY});
    Background_Loading.setSmooth(true);
    if (!Game_Icon.loadFromFile("Assets/Main menu & Loading/Loading Screen/Images/LOGO.png"))
    {
        cout << "!!! Error: Could not find LOGO.png !!!" << endl;
    }
    float width = 300;
    float height = 150;
    Game_Icon_sprite.setTexture(Game_Icon);
    Game_Icon_sprite.setPosition({(WindowWidth / 2) - 155, 7});
    texture_Size = Game_Icon.getSize();
    ScaleX = width / Game_Icon.getSize().x;
    ScaleY = height / Game_Icon.getSize().y;
    Game_Icon_sprite.setScale({ScaleX, ScaleY});
    Game_Icon.setSmooth(true);
    if (!POPCAP_Logo.loadFromFile("Assets/Main menu & Loading/Loading Screen/Images/popCapLogo.png"))
    {
        cout << "!!! Error: Could not find popCapLogo.png !!!" << endl;
    }
    width = 120;
    height = 110;
    POPCAP_Logo_sprite.setTexture(POPCAP_Logo);
    POPCAP_Logo_sprite.setPosition({(WindowWidth - 155), WindowHeight - 126.f});
    texture_Size = POPCAP_Logo.getSize();
    ScaleX = width / POPCAP_Logo.getSize().x;
    ScaleY = height / POPCAP_Logo.getSize().y;
    POPCAP_Logo_sprite.setScale({ScaleX, ScaleY});
    POPCAP_Logo.setSmooth(true);
    if (!LogoWood.loadFromFile("Assets/Main menu & Loading/Loading Screen/Images/sproutLogoWood.png"))
    {
        cout << "!!! Error: Could not find sproutLogoWood.png !!!" << endl;
    }
    width = 160;
    height = 80;
    LogoWood_sprite.setTexture(LogoWood);
    LogoWood_sprite.setPosition({(0 + 25), WindowHeight - 115.f});
    texture_Size = LogoWood.getSize();
    ScaleX = width / LogoWood.getSize().x;
    ScaleY = height / LogoWood.getSize().y;
    LogoWood_sprite.setScale({ScaleX, ScaleY});
    LogoWood.setSmooth(true);
    if (!fontTexture.loadFromFile("Assets/Fonts/LOADING.png"))
    {
        cout << "!!! Error: Could not find LOADING.png !!!" << endl;
    }
    fontTexture.setSmooth(true);
    float cellWidth = 19.4;
    float cellHeight = 35.5;
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
    bubbleSprite.setScale({0.7f, 0.7f});
    bubbleSprite.setOrigin({bubbleSprite.getLocalBounds().size.x / 2.f, bubbleSprite.getLocalBounds().size.y / 2.f});
    width_bubble_global = bubbleSprite.getGlobalBounds().size.x;
    bubbleTexture.setSmooth(true);
    width = 320.f;
    height = 50.f;
    loadBar_empty_sprite.setTexture(loadBar_empty);
    loadBar_empty_sprite.setOrigin({loadBar_empty_sprite.getLocalBounds().size.x / 2.f, loadBar_empty_sprite.getLocalBounds().size.y / 2.f});
    loadBar_empty_sprite.setPosition({WindowWidth / 2.f, WindowHeight - 85.f});
    texture_Size = loadBar_empty.getSize();
    ScaleX = width / loadBar_empty.getSize().x;
    ScaleY = height / loadBar_empty.getSize().y;
    loadBar_empty_sprite.setScale({ScaleX, ScaleY});
    loadBar_empty.setSmooth(true);
    loadBar_sprite.setTexture(loadBar);
    loadBar_sprite.setOrigin({loadBar_sprite.getLocalBounds().size.x / 2.f, loadBar_sprite.getLocalBounds().size.y / 2.f});
    loadBar_sprite.setPosition({WindowWidth / 2.f, WindowHeight - 85.f});
    loadBar_sprite.setScale({width / loadBar.getSize().x, height / loadBar.getSize().y});
    loadBar.setSmooth(true);
    bubbleSmallTex.setSmooth(true);
    spark.setSmooth(true);
}

void LoadingScreen()
{
    StartLoadingScreen();
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
    loadBar_sprite.setTextureRect(IntRect({0, 0}, {currentWidth, (int)textureSize.y}));
    window.draw(loadBar_empty_sprite);
    window.draw(loadBar_sprite);

    startX = WindowWidth / 2.0f - (3.5f * spacing) + 20.f;
    Sprite *letters[] = {&s_L, &s_O, &s_A, &s_D, &s_I, &s_N, &s_G};

    for (int i = 0; i < 7; i++)
    {
        float startDelay = 1.5f;
        float letterDelay = i * 0.4f; 
        float letterTime = totalTime - startDelay - letterDelay;
        float popScale = 0.0f;
        if (letterTime > 0)
        {
            float popDuration = 1.0f; 
            popScale = (letterTime < popDuration) ? sin(letterTime * (3.14f / popDuration)) * 1.2f : 1.0f;

            if (popScale < 1.0f && letterTime > (popDuration / 2.0f))
                popScale = 1.0f;
        }

        float curX = startX + (i * spacing);
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
        Bubbledone.play(); 
        exploded = true;
        for (int i = 0; i < 35; i++)
        {
            int idx = i / 5;
            float pX = startX + (idx * spacing);
            float pY = (WindowHeight / 2.0f - 40.f);

            
            sparks[i].active = true;
            sparks[i].x = pX;
            sparks[i].y = pY;

           
            sparks[i].vx = (float)(rand() % 50 - 25) / 30.0f;
            sparks[i].vy = -((float)(rand() % 50 + 20)) / 20.0f; 
            sparks[i].alpha = 255.0f;

            
            letterBubbles[i].active = true;
            letterBubbles[i].x = pX;
            letterBubbles[i].y = pY;

            
            letterBubbles[i].vx = (float)(rand() % 50 - 25) / 40.0f;

            
            letterBubbles[i].vy = -((float)(rand() % 30 + 10)) / 30.0f;

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
               
                sparks[i].vy += 0.04f;
                sparks[i].y += sparks[i].vy;
                
                sparks[i].alpha -= 1.5f;

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
                
                letterBubbles[i].alpha -= 1.75f;
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
        {
            particlesFinished = true;
        }
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
               
                ventBubbles[i].vx = (float)(rand() % 10 - 5) / 150.0f;
                ventBubbles[i].vy = -((float)(rand() % 20 + 20)) / 50.0f;
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
                ventBubbles[i].vx = -((float)(rand() % 30 + 15) / 20.f);
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
    view.setSize({WindowWidth, WindowHeight});
    view.setCenter({WindowWidth / 2.f, WindowHeight / 2.f});
    view.setViewport(FloatRect({0.f, 0.f}, {1.f, 1.f}));

    window.setView(view);
    StartMainMenu();

    FadeInFromBlack();
    playsound = 1;

    Clock frameClock;

    while (window.isOpen())
    {
        deltaTime = frameClock.restart().asSeconds();
        totaltime += deltaTime;

        if (isMusicEnabled && mainmenumusic.getStatus() != Music::Status::Playing)
        {
            mainmenumusic.play();
            mainmenumusic.setLooping(true);
        }

        while (const optional event = window.pollEvent())
        {
            if (event->is<Event::Closed>() || Keyboard::isKeyPressed(Keyboard::Key::Escape)){
                    QuitGame();
                    if (!window.isOpen()) return; 
                }
            if (auto mouseEvent = event->getIf<Event::MouseButtonReleased>())
            {
                if (mouseEvent->button == Mouse::Button::Left)
                {
                    Vector2f mousePos = window.mapPixelToCoords(Mouse::getPosition(window), view);

           
                    if (startgamebutton.getGlobalBounds().contains(mousePos))
                    {
                        currentGamemode = CLASSIC; 
                        isTimeAttackMode = false;  
                        FadeOutToBlack();
                        Select_level();
                        view.setSize({WindowWidth, WindowHeight});
                        view.setCenter({WindowWidth / 2.f, WindowHeight / 2.f});
                        FadeInFromBlack();
                        playsound = 1;
                    }
                    else if (timeattackbutton.getGlobalBounds().contains(mousePos))
                    {
                        currentGamemode = TIMEATTACK; 
                        isTimeAttackMode = true;      
                        FadeOutToBlack();
                        Select_level();
                        view.setSize({WindowWidth, WindowHeight});
                        view.setCenter({WindowWidth / 2.f, WindowHeight / 2.f});
                        FadeInFromBlack();
                        playsound = 1;
                    }

                    if (switchuserbutton.getGlobalBounds().contains(mousePos))
                    {
                        FadeOutToBlack();
                        SwitchUser();
                        FadeInFromBlack();
                        playsound = 1;
                        if (!CurUser.empty())
                        {
                            userName = CurUser;
                            welcomeLabel.setString("Welcome Back"); 
                        }
                        else
                        {
                            userName = "Guest";                
                            welcomeLabel.setString("Welcome"); 
                        }

                        
                        userNameText.setString(userName);

                       
                        userNameText.setOrigin(userNameText.getLocalBounds().size / 2.0f);
                        FloatRect wBounds = welcomeLabel.getLocalBounds();
                        welcomeLabel.setOrigin({wBounds.size.x / 2.0f, wBounds.size.y / 2.0f});
                    }

                    if (optionsbutton.getGlobalBounds().contains(mousePos))
                    {
                        FadeOutToBlack();
                        OptionsMenu();
                        FadeInFromBlack();
                        playsound = 1;
                    }
                    if (quitbutton.getGlobalBounds().contains(mousePos))
                    {
                        FadeOutToBlack();
                        QuitGame();
                        FadeInFromBlack();
                        playsound = 1;
                    }
                    if (highscorebutton.getGlobalBounds().contains(mousePos))
                    {
                        FadeOutToBlack();
                        Highscore();
                        FadeInFromBlack();
                        playsound = 1;
                    }
                    if (creditsbutton.getGlobalBounds().contains(mousePos))
                    {
                        FadeOutToBlack();
                        Credits();
                        FadeInFromBlack();
                        playsound = 1;
                    }
                }
            }
            else if (const auto *resizeEvent = event->getIf<Event::Resized>())
            {
                view.setViewport(sf::FloatRect({0.f, 0.f}, {1.f, 1.f}));
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
    view.setSize({800.f, 600.f});
    view.setCenter({400.f, 300.f});
    view.setViewport(FloatRect({0.f, 0.f}, {1.f, 1.f}));
    window.setView(view);

    if (!swayShader.loadFromFile("sway.frag", Shader::Type::Fragment))
    {
        cout << "Error loading shader!" << endl;
    }

    if (!CurUser.empty())
    {
       
        userName = CurUser;
        welcomeLabel.setString("Welcome Back"); 
    }
    else
    {
       
        userName = "Guest";                
        welcomeLabel.setString("Welcome"); 
    }
    // ==========================================

    smallfishs.clear();
    if (isMusicEnabled && mainmenumusic.getStatus() != Music::Status::Playing)
        mainmenumusic.play();
    mainmenumusic.setLooping(true);
    srand(time(0));
    for (int i = 0; i < 8; i++)
    {
        AnimatedObject obj{minowfishtex, 286, 126};
        float speedX = getRandom(1.5f, 3.f);
        float speedY = getRandom(0.5f, 2.f);
        obj.velocityX_AXIS = (rand() % 2 == 0) ? speedX : -speedX;
        obj.velocityY_AXIS = (rand() % 2 == 0) ? speedY : -speedY;
        if (i == 2 || i == 4 || i == 6)
        {
            obj.velocityY_AXIS = -obj.velocityY_AXIS;
        }
        obj.shape.setFillColor(Color::Green);
        obj.shape.setSize(Vector2f{57, 25});
        obj.shape.setOrigin(obj.shape.getLocalBounds().size / 2.f);
        obj.sprite.setTexture(minowfishtex);
        obj.sprite.setOrigin({285 / 2, 126 / 2});
        obj.sprite.setPosition(Vector2f({WindowWidth / 2.0f + (i * 100), WindowHeight / 2.0f + (i * rand() % 100)}));
        obj.changedir = (obj.velocityX_AXIS > 0) ? -1.f : 1.f;
        obj.sprite.setScale({0.2f * obj.changedir, 0.2f});
        smallfishs.push_back(obj);
    }
    welcometex.setSmooth(true);
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
    background.setPosition(Vector2f({WindowWidth / 2.0f, WindowHeight / 2.0f}));
    logosp.setOrigin(logosp.getLocalBounds().size / 2.0f);
    logosp.setPosition(Vector2f({WindowWidth / 2.0f, WindowHeight / 2.0f - 200}));
    Greenfish.setPosition(Vector2f({WindowWidth / 2.0f - 300, WindowHeight / 2.0f}));
    startgamebutton.setOrigin(startgamebutton.getLocalBounds().size / 2.0f);
    startgamebutton.setPosition(Vector2f({WindowWidth / 2.0f, WindowHeight / 2.0f - 50}));
    timeattackbutton.setOrigin(timeattackbutton.getLocalBounds().size / 2.0f);
    timeattackbutton.setPosition(Vector2f({WindowWidth / 2.0f + 10, WindowHeight / 2.0f + 50}));
    highscorebutton.setOrigin(highscorebutton.getLocalBounds().size / 2.0f);
    highscorebutton.setPosition(Vector2f({WindowWidth / 2.0f + -150, WindowHeight / 2.0f + 140}));
    optionsbutton.setOrigin(optionsbutton.getLocalBounds().size / 2.0f);
    optionsbutton.setPosition(Vector2f({WindowWidth / 2.0f + 120, WindowHeight / 2.0f + 150}));
    quitbutton.setOrigin(quitbutton.getLocalBounds().size / 2.0f);
    quitbutton.setPosition(Vector2f({WindowWidth / 2.0f, WindowHeight / 2.0f + 230}));
    switchuserbutton.setOrigin(switchuserbutton.getLocalBounds().size / 2.0f);
    switchuserbutton.setPosition(Vector2f({WindowWidth / 2.0f + 300, WindowHeight / 2.0f + 250}));
    creditsbutton.setOrigin(creditsbutton.getLocalBounds().size / 2.0f);
    creditsbutton.setPosition(Vector2f({WindowWidth / 2.0f - 300, WindowHeight / 2.0f + 240}));
    Barracuda.setPosition(Vector2f({WindowWidth / 2.f - 700, WindowHeight / 2.f - 200}));
    QueenTrigger.setPosition({1450, 360});
    Greenfish.setScale({GFchangedir, 1});
    Minowfish.setScale({0.2f * MFchangedir, 0.2f}); // 0.2 is the size factor for Minow
    Barracuda.setScale({BFchangedir, 1});
    QueenTrigger.setScale({QTchangedir, 1});

    mGreen.sprite = &Greenfish;
    mGreen.velocityX = &GFvelocityX_AXIS;
    mGreen.velocityY = &GFvelocityY_AXIS;
    mGreen.changedir = &GFchangedir;

    mMinow.sprite = &Minowfish;
    mMinow.velocityX = &MFvelocityX_AXIS;
    mMinow.velocityY = &MFvelocityY_AXIS;
    mMinow.changedir = &MFchangedir;

    mCuda.sprite = &Barracuda;
    mCuda.velocityX = &BFvelocityX_AXIS;
    mCuda.velocityY = &BFvelocityY_AXIS;
    mCuda.changedir = &BFchangedir;

    mQueen.sprite = &QueenTrigger;
    mQueen.velocityX = &QTvelocityX_AXIS;
    mQueen.velocityY = &QTvelocityY_AXIS;
    mQueen.changedir = &QTchangedir;

    // Make sure they start with clean turning states
    mGreen.turning = mMinow.turning = mCuda.turning = mQueen.turning = false;
    mGreen.turnFrame = mMinow.turnFrame = mCuda.turnFrame = mQueen.turnFrame = 0;

    // welcome text
    welcomeLabel.setFont(mainFont);
    welcomeLabel.setCharacterSize(18);
    welcomeLabel.setFillColor(Color::White);
    welcomeLabel.setOutlineColor(Color::Black);
    welcomeLabel.setOutlineThickness(1);
    welcomeLabel.setOrigin(welcomeLabel.getLocalBounds().size / 2.0f);
    welcomeLabel.setPosition(Vector2f({WindowWidth / 2.0f + 300, WindowHeight / 2.0f + 165}));

    // user name text
    userNameText.setFont(mainFont);
    userNameText.setString(userName);
    userNameText.setCharacterSize(16);
    userNameText.setFillColor(Color(180, 255, 100));
    userNameText.setOutlineColor(Color::Black);
    userNameText.setOutlineThickness(1);
    userNameText.setOrigin(userNameText.getLocalBounds().size / 2.0f);
    userNameText.setPosition(Vector2f({WindowWidth / 2.0f + 300, WindowHeight / 2.0f + 190}));

    welcomebutton.setOrigin(welcomebutton.getLocalBounds().size / 2.0f);
    welcomebutton.setPosition(Vector2f({WindowWidth / 2.0f + 300, WindowHeight / 2.0f + 180}));
}

void UpdateMainMenu()
{
    mermaidevent.stop();
    UpdateAmbientSounds();

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
    for (auto &obj : smallfishs)
    {
        obj.shape.setPosition({obj.sprite.getPosition().x, obj.sprite.getPosition().y});
        obj.update(286, 126, deltaTime);
        float posX = obj.sprite.getPosition().x;
        float posY = obj.sprite.getPosition().y;
        if (posX <= -150.f || posX >= WindowWidth + 150.f)
        {
            obj.velocityX_AXIS *= -1;
            if (posX <= -150.f)
                obj.sprite.setPosition({-149.f, posY});
            else
                obj.sprite.setPosition({WindowWidth + 149.f, posY});
        }
        if (posY <= -100.f || posY >= WindowHeight + 100.f)
        {
            obj.velocityY_AXIS *= -1;
            if (posY <= -100.f)
                obj.sprite.setPosition({posX, -99.f});
            else
                obj.sprite.setPosition({posX, WindowHeight + 99.f});
        }
    }
    // Re-center if the name length changed
    userNameText.setOrigin(userNameText.getLocalBounds().size / 2.0f);
    PlayingSound(true);
    ChangingButtonShape();
    BarracudaFishanimation(mCuda);
    QueenTriggerFish(mQueen);
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
        timeattackbutton.setTexture(timeattackpressed, true);
    else
        timeattackbutton.setTexture(timeattacktex, true);
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
        switchuserbutton.setTexture(switchuserpressed, true);
    else
        switchuserbutton.setTexture(switchusertex);
    if (creditsbutton.getGlobalBounds().contains(mouseWorldPos))
        creditsbutton.setTexture(creditspressed, true);
    else
        creditsbutton.setTexture(creditstex, true);
}

void DrawMainMenuBackground()
{
    window.setView(view);
    window.clear();
    swayShader.setUniform("time", totaltime);
    window.draw(background, &swayShader);
    window.draw(QueenTrigger);
    for (auto &obj : smallfishs)
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
    window.draw(welcomebutton);
    window.draw(welcomeLabel);
    window.draw(userNameText);
    window.display();
}

void MainMenuFishAnimation()
{
    GreenfishAnimation(mGreen);
    MinowFishanimation(mMinow);
    BarracudaFishanimation(mCuda);
    QueenTriggerFish(mQueen);
}

void GreenfishAnimation(MenuFish &fish)
{
    Greenfish.move({*fish.velocityX * deltaTime * 60.f,
                    *fish.velocityY * deltaTime * 60.f});

    float leftBound = -150.f;
    float rightBound = WindowWidth + 150.f;
    float posX = Greenfish.getPosition().x;

    // Turn when reaching edge and still moving outward
    if (!fish.turning &&
        ((posX <= leftBound && *fish.velocityX < 0) ||
         (posX >= rightBound && *fish.velocityX > 0)))
    {
        fish.turning = true;
        fish.turnFrame = 0;
    }

    if (fish.turning)
    {
        Greenfish.setTexture(Greenfishturn);
        Greenfish.setTextureRect(IntRect({fish.turnFrame * 227, 0}, {227, 255}));
        fish.turnFrame++;

        if (fish.turnFrame >= 13) // 13 frames in turn animation
        {
            fish.turnFrame = 0;
            fish.turning = false;

            *fish.changedir *= -1;
            Greenfish.setScale({*fish.changedir, 1});
            *fish.velocityX = -*fish.velocityX;
        }
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

void CheckHoverSound(Vector2f mousePos, std::initializer_list<FloatRect> rects)
{
    bool isHovering = false;
    for (const auto& r : rects)
        if (r.contains(mousePos))
        { isHovering = true; break; }

    if (isHovering)
    { if (playsound) { buttonpressedsound.play(); playsound = 0; } }
    else
        playsound = 1;
}

void PlayingSound(bool isMainMenu)
{
    if (isLevelRunning)
    {
        playsound = 1;
        return;
    }
    Vector2i mouseLocalPos = Mouse::getPosition(window);
    Vector2f mouseWorldPos = window.mapPixelToCoords(mouseLocalPos, view);
    bool isHovering = false;
    
    if (isMainMenu)
    {
        isHovering = (startgamebutton.getGlobalBounds().contains(mouseWorldPos) ||
                      timeattackbutton.getGlobalBounds().contains(mouseWorldPos) ||
                      highscorebutton.getGlobalBounds().contains(mouseWorldPos) ||
                      optionsbutton.getGlobalBounds().contains(mouseWorldPos) ||
                      quitbutton.getGlobalBounds().contains(mouseWorldPos) ||
                      switchuserbutton.getGlobalBounds().contains(mouseWorldPos) ||
                      creditsbutton.getGlobalBounds().contains(mouseWorldPos));
    }
    else
    {
        isHovering = NewButton.getGlobalBounds().contains(mouseWorldPos) ||
                     SelectButton.getGlobalBounds().contains(mouseWorldPos) ||
                     DeleteButton.getGlobalBounds().contains(mouseWorldPos) ||
                     (NameEntry && DoneAddingUser.getGlobalBounds().contains(mouseWorldPos)) ||
                     (NameEntry && CancelAddingUser.getGlobalBounds().contains(mouseWorldPos)) ||
                     (isConfirmUserDelete && YesButton.getGlobalBounds().contains(mouseWorldPos)) ||
                     (isConfirmUserDelete && NoButton.getGlobalBounds().contains(mouseWorldPos)) ||
                     (isListFull && FullOKButton.getGlobalBounds().contains(mouseWorldPos)) ||
                     (DupplicateName && DupplicateOKButton.getGlobalBounds().contains(mouseWorldPos));
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
        playsound = 1;
}

void MinowFishanimation(MenuFish &fish)
{
    if (!MFvisable)
        return;

    Minowfish.move({*fish.velocityX * deltaTime * 60.f, 0});

    const float leftBound = -180.f;
    const float rightBound = 980.f;
    float posX = Minowfish.getPosition().x;

    if (!fish.turning &&
        ((posX <= leftBound && *fish.velocityX < 0) ||
         (posX >= rightBound && *fish.velocityX > 0)))
    {
        fish.turning = true;
        fish.turnFrame = 0;
    }

    if (fish.turning)
    {
        Minowfish.setTextureRect(IntRect({fish.turnFrame * 286, 1 * 126}, {286, 126}));
        fish.turnFrame++;

        if (fish.turnFrame >= 6) // 6 frames in turn row
        {
            fish.turnFrame = 0;
            fish.turning = false;

            *fish.changedir *= -1;
            Minowfish.setScale({0.2f * (*fish.changedir), 0.2f});
            *fish.velocityX = -*fish.velocityX;
        }
    }
    else
    {
        Minowfish.setTextureRect(IntRect({MFcol * 286, 0}, {286, 126}));
        MFcol = (MFcol + 1) % 15;
    }
}

void BarracudaFishanimation(MenuFish &fish)
{
    Barracuda.move({*fish.velocityX * deltaTime * 60.f, 0});

    const float leftBound = -150.f;
    const float rightBound = WindowWidth + 150.f;
    float posX = Barracuda.getPosition().x;

    if (!fish.turning &&
        ((posX <= leftBound && *fish.velocityX < 0) ||
         (posX >= rightBound && *fish.velocityX > 0)))
    {
        fish.turning = true;
        fish.turnFrame = 0;
    }

    static float bAnimTimer = 0.f;
    bAnimTimer += deltaTime;
    if (bAnimTimer >= 0.08f)
    {
        bAnimTimer = 0.f;
        if (fish.turning)
        {
            Barracuda.setTextureRect(IntRect({fish.turnFrame * 256, 2 * 118}, {256, 118}));
            fish.turnFrame++;

            if (fish.turnFrame >= 5)
            {
                fish.turnFrame = 0;
                fish.turning = false;

                *fish.changedir *= -1;
                Barracuda.setScale({*fish.changedir, 1});
                *fish.velocityX = -*fish.velocityX;
            }
        }
        else if (Barracudacollieder.getGlobalBounds().findIntersection(MFcollieder.getGlobalBounds()))
        {
            // Use a static counter to track how far along the eat animation is
            static int bEatCounter = 0;

            if (bEatCounter < 1)
                BFcol = 0; // Start at the very first frame of the Eat animation

            if (bEatCounter == 3)
            {
                // Right as the jaws snap shut, teleport the Minnow off-screen
                Minowfish.setPosition({ 3000.f, 3000.f });
                MFvisable = 0;
            }

            bEatCounter++;

            if (bEatCounter <= 6) // Play 6 frames of the eating animation
            {
                // Row 0 is the Eat animation
                Barracuda.setTextureRect(IntRect({ BFcol * 256, 0 }, { 256, 118 }));
                BFcol++;
            }
            else
            {
                // Once eating is finished, immediately go back to the Swim animation (Row 1)
                Barracuda.setTextureRect(IntRect({ BFcol * 256, 1 * 118 }, { 256, 118 }));
                BFcol = (BFcol + 1) % 14;
                bEatCounter = 0; // Reset the counter just in case it needs to eat again
            }
        }
        else
        {
            Barracuda.setTextureRect(IntRect({BFcol * 256, 1 * 118}, {256, 118}));
            BFcol = (BFcol + 1) % 14;
        }
    }
}

void QueenTriggerFish(MenuFish &fish)
{
    
    QueenTrigger.move({*fish.velocityX * deltaTime * 60.f, *fish.velocityY * deltaTime * 60.f});

    const float leftBound = -150.f;
    const float rightBound = WindowWidth + 150.f;
    float posX = QueenTrigger.getPosition().x;
    float posY = QueenTrigger.getPosition().y;

    
    if (posY <= -100.f || posY >= WindowHeight + 100.f)
    {
        *fish.velocityY *= -1;
        // Keep it safely inside the bounds so it doesn't get stuck
        if (posY <= -100.f)
            QueenTrigger.setPosition({posX, -99.f});
        if (posY >= WindowHeight + 100.f)
            QueenTrigger.setPosition({posX, WindowHeight + 99.f});
    }

    if (!fish.turning &&
        ((posX <= leftBound && *fish.velocityX < 0) ||
         (posX >= rightBound && *fish.velocityX > 0)))
    {
        fish.turning = true;
        fish.turnFrame = 0;
    }

    
    static float qAnimTimer = 0.f;
    qAnimTimer += deltaTime;

    if (qAnimTimer >= 0.08f)
    {
        qAnimTimer = 0.f; // Reset the timer

        if (fish.turning)
        {
            QueenTrigger.setTextureRect(IntRect({fish.turnFrame * 298, 2 * 216}, {298, 216}));
            fish.turnFrame++;

            if (fish.turnFrame >= 6) // 6-turn frames
            {
                fish.turnFrame = 0;
                fish.turning = false;

                *fish.changedir *= -1;
                QueenTrigger.setScale({*fish.changedir, 1});
                *fish.velocityX = -*fish.velocityX;
            }
        }
        else if (QueenTriggercollieder.getGlobalBounds().findIntersection(smallfishs[0].sprite.getGlobalBounds()) ||
                 QueenTriggercollieder.getGlobalBounds().findIntersection(smallfishs[2].sprite.getGlobalBounds()))
        {
            static int counter = 0;
            if (counter < 1)
                QTcol = 0;
            if (counter == 3)
            {
                smallfishs[2].sprite.setPosition({3000, 3000});
                smallfishs[0].sprite.setPosition({3000, 3000});
            }
            counter++;
            if (counter <= 6)
            {
                QueenTrigger.setTextureRect(IntRect({QTcol * 298, 0}, {298, 216}));
                QTcol++;
            }
        }
        else
        {
            QueenTrigger.setTextureRect(IntRect({QTcol * 298, 1 * 216}, {298, 216}));
            QTcol = (QTcol + 1) % 14;
        }
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
        BarracudaFishanimation(mCuda);
        QueenTriggerFish(mQueen);
        for (auto &obj : smallfishs)
        {
            obj.shape.setPosition({obj.sprite.getPosition().x, obj.sprite.getPosition().y});
            obj.update(286, 126, deltaTime);
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
    CreateButton(Full, FullTex, "Assets/Switch User/listofusers.png", X, Y, 0.2, 0.2);
    X = WindowWidth * 0.25f, Y = WindowHeight * 0.85f;
    CreateButton(NewButton, NewButtonTex, "Assets/Switch User/Button.png", X, Y, 1.2, 1.2);
    if (!NewButtonHLTex.loadFromFile("Assets/Switch User/Button High.png"))
        cout << "Failed to load: ButtonHL" << "\n";
    X = WindowWidth * 0.5f, Y = WindowHeight * 0.85f;
    CreateButton(SelectButton, SelectButtonTex, "Assets/Switch User/Button.png", X, Y, 1.2, 1.2);
    if (!SelectButtonHLTex.loadFromFile("Assets/Switch User/Button High.png"))
        cout << "Failed to load: ButtonHL" << "\n";
    X = WindowWidth * 0.75f, Y = WindowHeight * 0.85f;
    CreateButton(DeleteButton, DeleteButtonTex, "Assets/Switch User/Button.png", X, Y, 1.2, 1.2);
    if (!DeleteButtonHLTex.loadFromFile("Assets/Switch User/Button High.png"))
        cout << "Failed to load: ButtonHL" << "\n";
    if (!btnFont.openFromFile("Assets/Fonts/BarmenoBold.otf"))
        cout << "Can't load font\n";
    SetupButtonText(NewText, "New", NewButton);
    SetupButtonText(SelectText, "Select", SelectButton);
    SetupButtonText(DeleteText, "Delete", DeleteButton);
    X = WindowWidth * 0.5f, Y = WindowHeight * 0.175f;

    RefreshUsersList();
    SelectUserHL.setSize({WindowWidth * 0.35f, 45.f});
    SelectUserHL.setFillColor(Color(180, 220, 255, 150));
    SelectUserHL.setOrigin({SelectUserHL.getSize().x / 2.f, 0.f});
}

void SetupButtonText(Text &text, const string &str, Sprite &button)
{
    text.setFont(btnFont);
    text.setString(str);
    text.setCharacterSize(30);
    text.setFillColor(Color(210, 240, 90));
    text.setOutlineColor(Color(30, 60, 10));
    text.setOutlineThickness(2.f);
    FloatRect bounds = text.getLocalBounds();
    text.setOrigin({bounds.size.x / 2.f, bounds.size.y / 2.f});
    Vector2f center = {button.getPosition().x, button.getPosition().y - 10};
    text.setPosition(center);
}

void CreateButton(Sprite &sprite, Texture &texture, const string &filePath,
                  float xPosition, float yPositon, float scaleX, float scaleY)
{
    if (!texture.loadFromFile(filePath))
        cout << "Failed to load: " << filePath << "\n";
    sprite = Sprite(texture);
    sprite.setOrigin({texture.getSize().x / 2.f, texture.getSize().y / 2.f});
    sprite.setPosition({xPosition, yPositon});
    sprite.setScale({scaleX, scaleY});
}

void UpdateSwitchUser()
{
    PlayingSound(false);
    Vector2f mousePos = window.mapPixelToCoords(Mouse::getPosition(window), view);
    while (auto event = window.pollEvent())
    {
        if (event->is<Event::Closed>())
            window.close();
        else if (const auto *resizeEvent = event->getIf<Event::Resized>())
        {
            view.setViewport(FloatRect({0.f, 0.f}, {1.f, 1.f}));
            window.setView(view);
        }
        if (const auto *keyPressed = event->getIf<Event::KeyPressed>())
        {
            if (keyPressed->code == Keyboard::Key::Escape){
                window.setView(view);
                MainMenu();
            }
        }
        if (auto mouseEvent = event->getIf<Event::MouseButtonReleased>())
            if (mouseEvent->button == Mouse::Button::Left)
            {
                if (NewButton.getGlobalBounds().contains(mousePos) && !isConfirmUserDelete && !isListFull)
                    EnterYourName();
                if (NameEntry && DoneAddingUser.getGlobalBounds().contains(mousePos))
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

                                // ---> NEW USERS START FRESH! <---
                                players[NumberOfUsers].level1Unlocked = true;
                                players[NumberOfUsers].level2Unlocked = false;
                                players[NumberOfUsers].level3Unlocked = false;
                                players[NumberOfUsers].ta_level1Unlocked = true;
                                players[NumberOfUsers].ta_level2Unlocked = false;
                                players[NumberOfUsers].ta_level3Unlocked = false;

                                NumberOfUsers++;
                                InputString = "";
                                RefreshUsersList();
                                CurUser = players[NumberOfUsers - 1].name;

                                // ---> SYNC RUNTIME MAP <---
                                level1Unlocked = true;
                                level2Unlocked = false;
                                level3Unlocked = false;
                                ta_level1Unlocked = true;
                                ta_level2Unlocked = false;
                                ta_level3Unlocked = false;
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
                else if (NameEntry && CancelAddingUser.getGlobalBounds().contains(mousePos))
                {
                    InputString = "";
                    NameEntry = 0;
                    isCancelAddingUser = 1;
                    DupplicateName = 0;
                    CamefromDupplicate = 0;
                    isUserSelected = 0;
                    SelectedUser = -1;
                }
                for (int i = 0; i < NumberOfUsers; i++)
                {
                    if (PlayersTexts[i] != nullptr && PlayersTexts[i]->getGlobalBounds().contains(mousePos))
                    {
                        SelectedUser = i;
                        float X = WindowWidth * 0.5f - 5, Y = PlayersTexts[i]->getPosition().y - 5.f;
                        SelectUserHL.setPosition({X, Y});
                    }
                }
                if (SelectButton.getGlobalBounds().contains(mousePos) && !NameEntry && !isListFull && !DupplicateName && SelectedUser >= 0 && SelectedUser < NumberOfUsers)
                {
                    CurUser = players[SelectedUser].name;
                    isUserSelected = 1;

                    // ---> SYNC MAP TO THE SELECTED USER <---
                    level1Unlocked = players[SelectedUser].level1Unlocked;
                    level2Unlocked = players[SelectedUser].level2Unlocked;
                    level3Unlocked = players[SelectedUser].level3Unlocked;
                    ta_level1Unlocked = players[SelectedUser].ta_level1Unlocked;
                    ta_level2Unlocked = players[SelectedUser].ta_level2Unlocked;
                    ta_level3Unlocked = players[SelectedUser].ta_level3Unlocked;
                }
                if (SelectedUser != -1 && !NameEntry && !isConfirmUserDelete && DeleteButton.getGlobalBounds().contains(mousePos))
                    DeleteUser();
                if (isConfirmUserDelete)
                {
                    if (!JustOpenDeleteConfirm && NoButton.getGlobalBounds().contains(mousePos))
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
                        isConfirmUserDelete = 0;
                        SelectedUser = -1;
                        RefreshUsersList();
                    }
                    JustOpenDeleteConfirm = 0;
                }
                if (FullOKButton.getGlobalBounds().contains(mousePos))
                    if (isListFull && !NameEntry && !isConfirmUserDelete && !DupplicateName)
                        isListFull = 0;
                if (DupplicateOKButton.getGlobalBounds().contains(mousePos))
                {
                    if (DupplicateName && !isListFull && !isConfirmUserDelete && !NameEntry && !isUserSelected)
                    {
                        DupplicateName = 0;
                        CamefromDupplicate = 1;
                        NameEntry = 1;
                    }
                }
            }
        if (const auto *typed = event->getIf<Event::TextEntered>())
        {
            char32_t a = typed->unicode;
            if (a == 8 && !InputString.empty())
                InputString.pop_back();
            else if (a >= 32 && InputString.size() < 8)
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
        float X = DisplayText.getPosition().x + textBounds.size.x + 8, Y = DisplayText.getPosition().y + 25;
        Blink.setPosition({X, Y});
        DisplayText.setString(InputString);
    }
    HoverButton(NewButton, NewButtonTex, NewButtonHLTex, mousePos, NewText);
    HoverButton(SelectButton, NewButtonTex, NewButtonHLTex, mousePos, SelectText);
    HoverButton(DeleteButton, NewButtonTex, NewButtonHLTex, mousePos, DeleteText);
    if (NameEntry && !isConfirmUserDelete)
    {
        HoverButton(DoneAddingUser, DoneAddingUserTex, DoneAddingUserHLTex, mousePos, DoneAddingUserText);
        HoverButton(CancelAddingUser, CancelAddingUserTex, CancelAddingUserHLTex, mousePos, CancelAddingUserText);
    }
    if (isConfirmUserDelete && !NameEntry)
    {
        HoverButton(YesButton, NewButtonTex, NewButtonHLTex, mousePos, YesButtonText);
        HoverButton(NoButton, NewButtonTex, NewButtonHLTex, mousePos, NoButtonText);
    }
    if (!NameEntry && !isConfirmUserDelete && isListFull && !DupplicateName)
        HoverButton(FullOKButton, NewButtonTex, NewButtonHLTex, mousePos, OKText);
    if (!NameEntry && !isConfirmUserDelete && !isListFull && DupplicateName)
        HoverButton(DupplicateOKButton, NewButtonTex, NewButtonHLTex, mousePos, DupplicatedUserText);
}

void HoverButton(Sprite &button, const Texture &normalTex, const Texture &highlightTex, Vector2f mousePos, Text &text)
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
    PlayingSound(false);
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
    CreateButton(EnterYourNamebg, EnterYourNameBgTex, "Assets/Switch User/enteryourname.png", X, Y, 0.2, 0.2);
    if (!Inputfont.openFromFile("Assets/Fonts/trebuc.ttf"))
        cout << "Cant Open Font!";
    DisplayText.setFont(Inputfont);
    DisplayText.setCharacterSize(30);
    DisplayText.setFillColor(Color::White);
    X = WindowWidth * 0.28f, Y = WindowHeight * 0.34f;
    DisplayText.setPosition({(float)X, (float)Y});
    X = WindowWidth * 0.4f, Y = WindowHeight * 0.448f;
    CreateButton(Blink, BlinkTex, "Assets/Switch User/shell_editboxcursor.jpg", X, Y, 1, 1);
    X = WindowWidth * 0.5f, Y = WindowHeight * 0.62f;
    CreateButton(DoneAddingUser, DoneAddingUserTex, "Assets/Switch User/Button.png", X, Y, 1.25, 1.25);
    SetupButtonText(DoneAddingUserText, "Done", DoneAddingUser);
    if (!DoneAddingUserHLTex.loadFromFile("Assets/Switch User/Button High.png"))
        cout << "Failed to load: ButtonHL" << "\n";
    if (!CancelAddingUserHLTex.loadFromFile("Assets/Switch User/shell_tinybtn124_high.jpg"))
        cout << "Can't load Texture!\n";
    X = WindowWidth * 0.5f, Y = WindowHeight * 0.75f;
    CreateButton(CancelAddingUser, CancelAddingUserTex, "Assets/Switch User/Cancel.png", X, Y, 1.5, 1.5);
    SetupButtonText(CancelAddingUserText, "Cancel", CancelAddingUser);
    CancelAddingUserText.setPosition({CancelAddingUser.getPosition().x, CancelAddingUser.getPosition().y - 5});
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
        float Y = WindowHeight * 0.2f + (id * 40.f);
        FloatRect bounds = PlayersTexts[id]->getLocalBounds();
        PlayersTexts[id]->setOrigin({bounds.size.x / 2.f, 0.f});
        PlayersTexts[id]->setPosition({X, Y});
    }
}

void DeleteUser()
{
    JustOpenDeleteConfirm = 1;
    isConfirmUserDelete = 1;
    unsigned int X = WindowWidth * 0.5f, Y = WindowHeight * 0.5f;
    CreateButton(DeleteUserBg, DelteUserBgTex, "Assets/Switch User/deleteuser.png", X, Y, 0.175, 0.175);
    X = WindowWidth * 0.4f, Y = WindowHeight * 0.7f;
    CreateButton(YesButton, NewButtonTex, "Assets/Switch User/Button.png", X, Y, 1.25, 1.25);
    SetupButtonText(YesButtonText, "Yes", YesButton);
    X = WindowWidth * 0.6f, Y = WindowHeight * 0.7f;
    CreateButton(NoButton, NewButtonTex, "Assets/Switch User/Button.png", X, Y, 1.25, 1.25);
    SetupButtonText(NoButtonText, "No!", NoButton);
    SetupButtonText(DeletethisUser, players[SelectedUser].name, DeleteUserBg);
    X = WindowWidth * 0.47f, Y = WindowHeight * 0.52f;
    DeletethisUser.setPosition({(float)X, (float)Y});
    DeletethisUser.setCharacterSize(45);
}

void FullList()
{
    isListFull = 1;
    unsigned int X = WindowWidth * 0.5f, Y = WindowHeight * 0.5f;
    CreateButton(ListisFull, ListisFullTex, "Assets/Switch User/listisfull.png", X, Y, 0.2, 0.2);
    X = WindowWidth * 0.5f, Y = WindowHeight * 0.64f;
    CreateButton(FullOKButton, FullOKButtonTex, "Assets/Switch User/Button.png", X, Y, 1.5, 1.5);
    SetupButtonText(FullListOKBtnText, "OK", FullOKButton);
    InputString = "";
}

void DupplicateUser()
{
    DupplicateName = 1;
    unsigned int X = WindowWidth * 0.5f, Y = WindowHeight * 0.5f;
    CreateButton(DupplicateBg, DupplicateBgTex, "Assets/Switch User/dupplicateuser.png", X, Y, 0.2, 0.2);
    X = WindowWidth * 0.5f, Y = WindowHeight * 0.72f;
    CreateButton(DupplicateOKButton, DupplicateOKButtonTex, "Assets/Switch User/Button.png", X, Y, 1.5, 1.5);
    SetupButtonText(DupplicateOKBtnText, "OK", DupplicateOKButton);
    SetupButtonText(DupplicatedUserText, InputString, DupplicateBg);
    X = WindowWidth * 0.48f, Y = WindowHeight * 0.56f;
    DupplicatedUserText.setPosition({(float)X, (float)Y});
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
    if (SelectedUser >= 0)
        window.draw(SelectUserHL);
    for (int user = 0; user < NumberOfUsers; user++)
        if (PlayersTexts[user] != nullptr)
            window.draw(*PlayersTexts[user]);
    if (isUserSelected)
    {
        isUserSelected = 0;
        return;
    }
    if (NameEntry && !isConfirmUserDelete && !isListFull && !DupplicateName)
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
    if (isConfirmUserDelete && SelectedUser != -1 && !NameEntry && !isListFull && !DupplicateName)
    {
        DrawMainMenuBackground();
        window.draw(DeleteUserBg);
        window.draw(YesButton);
        window.draw(NoButton);
        window.draw(YesButtonText);
        window.draw(NoButtonText);
        window.draw(DeletethisUser);
    }
    if (isListFull && !isConfirmUserDelete && !NameEntry && !DupplicateName && !isUserSelected)
    {
        DrawMainMenuBackground();
        window.draw(ListisFull);
        window.draw(FullOKButton);
        window.draw(FullListOKBtnText);
    }
    if (DupplicateName && !isListFull && !isConfirmUserDelete && !NameEntry && !isUserSelected)
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
        deltaTime = clock.restart().asSeconds();
        totaltime += deltaTime;
        BarracudaFishanimation(mCuda);
        QueenTriggerFish(mQueen);
        for (auto &obj : smallfishs)
        {
            obj.shape.setPosition({obj.sprite.getPosition().x, obj.sprite.getPosition().y});
            obj.update(286, 126, deltaTime);
            float posX = obj.sprite.getPosition().x;
            if (posX <= -150.f || posX >= WindowWidth + 150.f)
            {
                obj.velocityX_AXIS *= -1;
            }
        }
        deltaTime = clock.restart().asSeconds();
        while (const optional event = window.pollEvent())
        {
            if (event->is<Event::Closed>())
                window.close();
            else if (const auto *resizeEvent = event->getIf<Event::Resized>())
            {
                view.setViewport(FloatRect({0.f, 0.f}, {1.f, 1.f}));
                window.setView(view);
            }
            if (const auto *keyPressed = event->getIf<Event::KeyPressed>())
            {
                if (keyPressed->code == Keyboard::Key::Escape)
                {
                    if (g_optionsFromPause)
                    {

                        shouldCloseOptions = true; 
                        return;
                    }
                    else
                    {
                        window.setView(view);
                        MainMenu();
                    }
                }
            }
        }
        UpdateOptions();
        if (shouldCloseOptions)
            return;
        DrawOptions();
    }
}

void setRightAligned(optional<Text> &txt, float x, float y)
{
    if (txt)
    {
        FloatRect bounds = txt->getLocalBounds();
        txt->setOrigin({bounds.size.x, bounds.size.y / 2.f});
        txt->setPosition({x, y});
    }
}

void setTopCentered(optional<Text> &txt, float x, float y)
{
    if (txt)
    {
        FloatRect bounds = txt->getLocalBounds();
        txt->setOrigin({bounds.size.x / 2.f, 0.f});
        txt->setPosition({x, y});
    }
}

void drawTextWithShadow(RenderWindow &win, optional<Text> &textObj)
{
    if (textObj)
    {
        Text shadow = *textObj;
        shadow.setFillColor(Color(10, 20, 40, 180));
        shadow.setOutlineThickness(0.f);
        shadow.move({1.f, 1.f});
        win.draw(shadow);
        win.draw(*textObj);
    }
}

void drawCenteredSprite(sf::RenderWindow &window, sf::Sprite &sprite, sf::Texture &texture, int opacity)
{
    sprite.setTexture(texture);
    sprite.setColor(sf::Color(255, 255, 255, opacity));
    auto textureSize = sf::Vector2f(texture.getSize());
    sprite.setOrigin(textureSize / 2.f);
    window.draw(sprite);
}

void StartOptions()
{
    playsound = 1; 
    static bool isInitialized = false;
    if (!isInitialized)
    {
        srand(static_cast<unsigned int>(time(NULL)));
        if (!globalFont.openFromFile("Assets/Fonts/Barmeno.ttf"))
            cout << "Failed to load font!" << endl; 
        globalFont.setSmooth(false);
        screenDarkener.setSize(Vector2f(WindowWidth, WindowHeight));
        screenDarkener.setFillColor(Color(0, 0, 0, 95));

        if (!barmenoBoldFont.openFromFile("Assets/Fonts/BarmenoBold.otf"))
            cout << "Failed to load Barmeno bold!" << endl;
        barmenoBoldFont.setSmooth(true);

        if (texBgPlank.loadFromFile("Assets/Options/plank.png"))
        { 
            texBgPlank.setSmooth(true);
            sprBgPlank.emplace(texBgPlank);
            FloatRect b = sprBgPlank->getLocalBounds();
            sprBgPlank->setOrigin({b.size.x / 2.f, b.size.y / 2.f});
            sprBgPlank->setPosition({WindowWidth / 2.f, 280.f});
        }
        if (texCorals.loadFromFile("Assets/Options/corals.png"))
        { 
            texCorals.setSmooth(true);
            sprCorals.emplace(texCorals);
            FloatRect b = sprCorals->getLocalBounds();
            sprCorals->setOrigin({b.size.x / 2.f, b.size.y / 2.f});
            sprCorals->setPosition({(WindowWidth - 2) / 2.f, 315.f});
            sprCorals->setColor(Color(255, 255, 255, 175));
        }
        if (texTitle.loadFromFile("Assets/Options/options_title.png"))
        { 
            texTitle.setSmooth(true);
            sprTitle.emplace(texTitle);
            FloatRect b = sprTitle->getLocalBounds();
            sprTitle->setOrigin({b.size.x / 2.f, b.size.y / 2.f});
            sprTitle->setPosition({WindowWidth / 2.f, 100.f});
            sprTitle->setScale({1.f, 1.f});
        }

        (void)texUncheckedNormal.loadFromFile("Assets/Options/_shell_checkbtn_normal.png");     
        (void)texUncheckedHover.loadFromFile("Assets/Options/_shell_checkbtn_high.png");        
        (void)texCheckedNormal.loadFromFile("Assets/Options/shell_checkbtnchecked_normal.png"); 
        (void)texCheckedHover.loadFromFile("Assets/Options/_shell_checkbtnchecked_high.png");   
        (void)texDoneNormal.loadFromFile("Assets/Options/done_normal.png");                     
        (void)texDoneHover.loadFromFile("Assets/Options/done_hover.png");                      
        texUncheckedNormal.setSmooth(true);
        texUncheckedHover.setSmooth(true);
        texCheckedNormal.setSmooth(true);
        texCheckedHover.setSmooth(true);
        texDoneNormal.setSmooth(true);
        texDoneHover.setSmooth(true);
        Color paleText(249, 251, 251);
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
        versionLabel->setPosition({170.f, 423.f});
        versionLabel->setFillColor(Color(222, 237, 244));
        const char *labels[] = {"Sound:", "Music:", "Full Screen:", "Slow", "", "", "Fast", "Low", "", "High", "Done"};
        for (int i = 0; i < NumOptions; i++)
        {
           
            if (i == 10)
                OptionButtons[i].text.emplace(barmenoBoldFont);
            else
                OptionButtons[i].text.emplace(globalFont);
        

            OptionButtons[i].text->setString(labels[i]);
            OptionButtons[i].isCheckbox = true;
            OptionButtons[i].isHovered = false;
            OptionButtons[i].checkbox.emplace(texUncheckedNormal);
            if (i < 3)
            {
                if (i == 0)
                    OptionButtons[i].isChecked = isSoundEnabled;
                else if (i == 1)
                    OptionButtons[i].isChecked = isMusicEnabled;
                else if (i == 2)
                    OptionButtons[i].isChecked = isFullscreen;

                OptionButtons[i].text->setCharacterSize(22);
                OptionButtons[i].text->setFillColor(paleText);
                float yPos = POS_Y_TOP_OPTIONS + (i * LAYOUT_ROW_SPACING);
                setRightAligned(OptionButtons[i].text, LAYOUT_LABEL_X, yPos);
                OptionButtons[i].checkbox->setPosition({LAYOUT_BUBBLE_START_X, yPos + 7.f});
            }
            else if (i >= 3 && i <= 6)
            {
                OptionButtons[i].isChecked = (i == (MSpeedIndex - 1 + 3));
                OptionButtons[i].text->setCharacterSize(14);
                OptionButtons[i].text->setFillColor(paleText);
                float startX = LAYOUT_BUBBLE_START_X + ((i - 3) * LAYOUT_COL_SPACING);
                OptionButtons[i].checkbox->setPosition({startX, POS_Y_MOUSE_SPEED + 7.f});
                setTopCentered(OptionButtons[i].text, startX, POS_Y_MOUSE_SPEED + 20.f);
            }
            else if (i >= 7 && i <= 9)
            {
                OptionButtons[i].isChecked = (i == (GraphicsIndex + 7));
                OptionButtons[i].text->setCharacterSize(14);
                OptionButtons[i].text->setFillColor(paleText);
                float startX = LAYOUT_BUBBLE_START_X + ((i - 7) * LAYOUT_COL_SPACING);
                OptionButtons[i].checkbox->setPosition({startX, POS_Y_GAME_DETAIL + 7.f});
                setTopCentered(OptionButtons[i].text, startX, POS_Y_GAME_DETAIL + 20.f);
            }
            else
            {
                OptionButtons[i].isCheckbox = false;
                OptionButtons[i].checkbox->setTexture(texDoneNormal, true);
                FloatRect plankBounds = OptionButtons[i].checkbox->getLocalBounds();
                OptionButtons[i].checkbox->setOrigin({plankBounds.size.x / 2.f, plankBounds.size.y / 2.f});
                OptionButtons[i].checkbox->setPosition({WindowWidth / 2.f, 450.f});

                
                OptionButtons[i].text->setCharacterSize(30);

                OptionButtons[i].text->setFillColor(Color(160, 211, 74));
                OptionButtons[i].text->setOutlineColor(Color(66, 50, 25));
                OptionButtons[i].text->setOutlineThickness(2.f);
                FloatRect textBounds = OptionButtons[i].text->getLocalBounds();
                OptionButtons[i].text->setOrigin({textBounds.size.x / 2.f, textBounds.size.y / 2.f});
                OptionButtons[i].text->setPosition({WindowWidth / 2.f, 440.f});
            }
        }
        isInitialized = true;
    }
    if (!g_optionsFromPause)
    {
        view.setSize({800.f, 600.f});
        view.setCenter({400.f, 300.f});
        view.setViewport(FloatRect({0.f, 0.f}, {1.f, 1.f}));
        window.setView(view);
    }
}
void UpdateOptions()
{
    // PlayingSound(false);
    Vector2f MousePosition;
    if (g_optionsFromPause)
    {
        // If in a level, calculate mouse based on the static screen
        MousePosition = window.mapPixelToCoords(Mouse::getPosition(window), uiView);
    }
    else
    {
        // If in main menu, use the standard view
        MousePosition = window.mapPixelToCoords(Mouse::getPosition(window), view);
    }

    CheckHoverSound(MousePosition, {
        OptionButtons[0].checkbox->getGlobalBounds(), // Sound
        OptionButtons[1].checkbox->getGlobalBounds(), // Music
        OptionButtons[2].checkbox->getGlobalBounds(), // Fullscreen
        OptionButtons[3].checkbox->getGlobalBounds(), // Slow
        OptionButtons[4].checkbox->getGlobalBounds(), // (bubble)
        OptionButtons[5].checkbox->getGlobalBounds(), // (bubble)
        OptionButtons[6].checkbox->getGlobalBounds(), // Fast
        OptionButtons[7].checkbox->getGlobalBounds(), // Low
        OptionButtons[8].checkbox->getGlobalBounds(), // (bubble)
        OptionButtons[9].checkbox->getGlobalBounds(), // High
        OptionButtons[10].checkbox->getGlobalBounds() // Done Button
    });

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
                    isFullscreen = OptionButtons[2].isChecked; 
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
                        window.create(VideoMode({800, 600}), "Feeding Frenzy 2", State::Windowed);

                    
                    view.setSize({800.f, 600.f});
                    view.setCenter({400.f, 300.f});
                    view.setViewport(FloatRect({0.f, 0.f}, {1.f, 1.f}));

                    window.setView(view);
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
                OptionButtons[i].text->setFillColor(Color(255, 241, 74));
            else
                OptionButtons[i].text->setFillColor(Color(160, 211, 74));
        }
    }

    isSoundEnabled = OptionButtons[0].isChecked;
    isMusicEnabled = OptionButtons[1].isChecked;
    isFullscreen = OptionButtons[2].isChecked;

    ApplyAudioSettings();

    prevMousePressed = currentMousePressed;
}

void DrawOptions()
{
    window.setView(view);
    
    if (g_optionsFromPause)
    {
        window.setView(view);

        if (isLevelRunning && !g_inEndScreen) 
            Drawbglevel();     
        else if (gameScreenActive)
            window.draw(gameScreenBgSprite); 
        else
            window.draw(sprEndBg); 

        window.setMouseCursorVisible(true);
        
        window.setView(uiView);
    }
    else
    {
        window.setView(view);
        DrawMainMenuBackground();
    }

    window.draw(screenDarkener);
    
    if (sprBgPlank)           
        window.draw(*sprBgPlank);
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
            OptionButtons[i].checkbox->setScale({1.0f, 1.0f});
            drawCenteredSprite(window, *OptionButtons[i].checkbox, texDoneNormal, 230);
            if (OptionButtons[i].isHovered)
            {
                OptionButtons[i].checkbox->setScale({1.05f, 1.05f});
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
                OptionButtons[i].checkbox->setScale({1.1f, 1.1f});
            else
                OptionButtons[i].checkbox->setScale({1.0f, 1.0f});
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
    GreenfishAnimation(mGreen);
    MinowFishanimation(mMinow);
    BarracudaFishanimation(mCuda);
    QueenTriggerFish(mQueen);
    for (auto &obj : smallfishs)
    {
        obj.shape.setPosition({obj.sprite.getPosition().x, obj.sprite.getPosition().y});
        obj.update(286, 126, deltaTime);
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
        deltaTime = clock.restart().asSeconds();
        totaltime += deltaTime;
        BarracudaFishanimation(mCuda);
        QueenTriggerFish(mQueen);
        for (auto &obj : smallfishs)
        {
            obj.shape.setPosition({obj.sprite.getPosition().x, obj.sprite.getPosition().y});
            obj.update(286, 126, deltaTime);
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
                    Vector2f yesPos = {WindowWidth / 2.f - 100.f, WindowHeight / 2.f + 90.f};
                    Vector2f noPos = {WindowWidth / 2.f + 100.f, WindowHeight / 2.f + 90.f};
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
            if (const auto *keyPressed = event->getIf<Event::KeyPressed>())
        {
            if (keyPressed->code == Keyboard::Key::Escape){
                window.setView(view);
                MainMenu();
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
    playsound = 1;
    if (!btnFont.openFromFile("Assets/Fonts/BarmenoBold.otf"))
    {
        cout << "Error: Could not load font!" << endl;
    }
    if (!quit_bg.loadFromFile("Assets/Quit/bg_quit.png"))
        cout << "Error Loading Quit BG!" << endl;
    quit_bg_sprite.setTexture(quit_bg);
    quit_bg.setSmooth(true);
    float targetWidth = 500.f;
    float targetHeight = 300.f;
    Vector2u texture_Size = quit_bg.getSize();
    float ScaleX = targetWidth / texture_Size.x;
    float ScaleY = targetHeight / texture_Size.y;
    quit_bg_sprite.setOrigin({texture_Size.x / 2.f, texture_Size.y / 2.f});
    quit_bg_sprite.setPosition({WindowWidth / 2.f, WindowHeight / 2.f});
    quit_bg_sprite.setScale({ScaleX, ScaleY});
    quit_icon_sprite.setOrigin({quit_icon.getSize().x / 2.f, quit_icon.getSize().y / 2.f});
    quit_icon_sprite.setPosition({WindowWidth / 2.f, WindowHeight / 2.f - 133.f});
    quit_icon_sprite.setScale({0.25f, 0.25f});
    quit_button.setSmooth(true);
    quit_button_hover.setSmooth(true);
    quit_button_sprite.setTexture(quit_button);
    quit_button_hover_sprite.setTexture(quit_button_hover);
    Vector2f btnSize = {(float)quit_button.getSize().x, (float)quit_button.getSize().y};
    quit_button_sprite.setOrigin({btnSize.x / 2.f, btnSize.y / 2.f});
    quit_button_hover_sprite.setOrigin({btnSize.x / 2.f, btnSize.y / 2.f});
    quit_title_text.setFont(btnFont);
    quit_title_text.setString("Are you sure you want to quit");
    quit_title_text.setCharacterSize(28);
    quit_title_text.setFillColor(Color(255, 255, 255));
    quit_title_text.setOutlineThickness(1.5f);
    quit_title_text.setOutlineColor(Color(0, 0, 0));
    FloatRect titleBounds = quit_title_text.getLocalBounds();
    quit_title_text.setOrigin({titleBounds.position.x + titleBounds.size.x / 2.f, titleBounds.position.y + titleBounds.size.y / 2.f});
    quit_title_text.setPosition({WindowWidth / 2.f, WindowHeight / 2.f - 30.f});
    quit_title_text2.setFont(btnFont);
    quit_title_text2.setString("Feeding Frenzy 2?");
    quit_title_text2.setCharacterSize(28);
    quit_title_text2.setFillColor(Color(255, 255, 255));
    quit_title_text2.setOutlineThickness(1.5f);
    quit_title_text2.setOutlineColor(Color(0, 0, 0));
    FloatRect titleBounds2 = quit_title_text2.getLocalBounds();
    quit_title_text2.setOrigin({titleBounds2.position.x + titleBounds2.size.x / 2.f, titleBounds2.position.y + titleBounds2.size.y / 2.f});
    quit_title_text2.setPosition({WindowWidth / 2.f, WindowHeight / 2.f + 10.f});
    quit_yes_text.setFont(btnFont);
    quit_yes_text.setString("Yes");
    quit_yes_text.setCharacterSize(24);
    quit_yes_text.setFillColor(Color(180, 255, 100));
    quit_yes_text.setOutlineThickness(2);
    quit_yes_text.setOutlineColor(Color(20, 60, 0));
    FloatRect yesBounds = quit_yes_text.getLocalBounds();
    quit_yes_text.setOrigin({yesBounds.position.x + yesBounds.size.x / 2.f, yesBounds.position.y + yesBounds.size.y / 2.f});
    quit_yes_text.setPosition({WindowWidth / 2.f - 100.f, WindowHeight / 2.f + 90.f});
    quit_no_text.setFont(btnFont);
    quit_no_text.setString("No!");
    quit_no_text.setCharacterSize(24);
    quit_no_text.setFillColor(Color(180, 255, 100));
    quit_no_text.setOutlineThickness(2);
    quit_no_text.setOutlineColor(Color(20, 60, 0));
    FloatRect noBounds = quit_no_text.getLocalBounds();
    quit_no_text.setOrigin({noBounds.position.x + noBounds.size.x / 2.f, noBounds.position.y + noBounds.size.y / 2.f});
    quit_no_text.setPosition({WindowWidth / 2.f + 100.f, WindowHeight / 2.f + 90.f});
    quit_yes_pressed = false;
    quit_no_pressed = false;
}

void UpdateQuit()
{
    // PlayingSound(false);
    Vector2f mousePos = window.mapPixelToCoords(Mouse::getPosition(window), view);
    Vector2f yesPos = {WindowWidth / 2.f - 100.f, WindowHeight / 2.f + 90.f};
    Vector2f noPos = {WindowWidth / 2.f + 100.f, WindowHeight / 2.f + 90.f};
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

    CheckHoverSound(mousePos, { yesBounds, noBounds });
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

void updateHighScoreTexts()
{
    HighScoreEntry *activeArr = (currentMode == STORY) ? story_scores : timeattack_scores;
    for (int i = 0; i < VISIBLE_SCORES; i++)
    {
        int idx = scrollOffset + i;
        textHSListRanks[i].setString(to_string(idx + 1) + ". ");
        textHSListNames[i].setString(activeArr[idx].name);
        textHSListScores[i].setString(formatScore(activeArr[idx].score));
        FloatRect sb = textHSListScores[i].getLocalBounds();
        textHSListScores[i].setOrigin({sb.size.x, 0.f});
        textHSListScores[i].setPosition({540.f, LIST_START_Y + (i * LIST_SPACING)});
    }
}

void resetScores()
{
    // Reset BOTH arrays back to default values
    for (int i = 0; i < MAX_SCORES; i++)
    {
        story_scores[i] = {"", 0};
        timeattack_scores[i] = {"", 0};
    }

    scrollOffset = 0;
    updateHighScoreTexts();
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
        BarracudaFishanimation(mCuda);
        QueenTriggerFish(mQueen);
        for (auto &obj : smallfishs)
        {
            obj.shape.setPosition({obj.sprite.getPosition().x, obj.sprite.getPosition().y});
            obj.update(286, 126, deltaTime);
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
    HighScoreEntry *activeList = isStoryMode ? story_scores : timeattack_scores;
    if (score < activeList[MAX_SCORES - 1].score)
        return;
    activeList[MAX_SCORES - 1].name = name;
    activeList[MAX_SCORES - 1].score = score;
    proceduralSort(activeList);
    updateHighScoreTexts();
}

void StartHighscore()
{
    playsound = 1;
    window.setView(view);
    texHSDoneNormal.setSmooth(true);
    texHSDoneHover.setSmooth(true);
    texHSResetNormal.setSmooth(true);
    texHSResetHover.setSmooth(true);
    texHSArrowNormal.setSmooth(true);
    texHSArrowHover.setSmooth(true);
    texHSArrowDown.setSmooth(true);
    sprHSDonePlank.setTexture(texHSDoneNormal);
    sprHSDonePlank.setOrigin({sprHSDonePlank.getLocalBounds().size.x / 2.f, sprHSDonePlank.getLocalBounds().size.y / 2.f});
    sprHSDonePlank.setPosition({400.f, 470.f});
    sprHSResetPlank.setTexture(texHSResetNormal);
    sprHSResetPlank.setOrigin({sprHSResetPlank.getLocalBounds().size.x / 2.f, sprHSResetPlank.getLocalBounds().size.y / 2.f});
    sprHSResetPlank.setPosition({400.f, 560.f});
    sprHSStoryArrowRight.setTexture(texHSArrowNormal, true);
    sprHSStoryArrowRight.setOrigin({texHSArrowNormal.getSize().x / 2.f, texHSArrowNormal.getSize().y / 2.f});
    sprHSStoryArrowRight.setPosition({480.f, 130.f});
    sprHSStoryArrowLeft.setTexture(texHSArrowNormal, true);
    sprHSStoryArrowLeft.setOrigin({texHSArrowNormal.getSize().x / 2.f, texHSArrowNormal.getSize().y / 2.f});
    sprHSStoryArrowLeft.setPosition({320.f, 130.f});
    sprHSStoryArrowLeft.setScale({-1.f, 1.f});
    sprHSListArrowUp.setTexture(texHSArrowNormal, true);
    sprHSListArrowUp.setOrigin({texHSArrowNormal.getSize().x / 2.f, texHSArrowNormal.getSize().y / 2.f});
    sprHSListArrowUp.setPosition({570.f, LIST_START_Y + 10.f});
    sprHSListArrowUp.setRotation(degrees(270.f));
    sprHSListArrowDown.setTexture(texHSArrowNormal, true);
    sprHSListArrowDown.setOrigin({texHSArrowNormal.getSize().x / 2.f, texHSArrowNormal.getSize().y / 2.f});
    sprHSListArrowDown.setPosition({570.f, LIST_START_Y + (9 * LIST_SPACING) + 10.f});
    sprHSListArrowDown.setRotation(degrees(90.f));
    textHSMainTitle.setFillColor(colorTitleGreen);
    textHSMainTitle.setOutlineThickness(2.f);
    textHSMainTitle.setOrigin({textHSMainTitle.getLocalBounds().size.x / 2.f, 0.f});
    textHSMainTitle.setPosition({400.f, 40.f});
    textHSStoryMode.setOrigin({textHSStoryMode.getLocalBounds().size.x / 2.f, 0.f});
    textHSStoryMode.setPosition({400.f, 115.f});
    HSModeUnderline.setFillColor(Color::White);
    HSModeUnderline.setSize({textHSStoryMode.getLocalBounds().size.x, 2.f});
    HSModeUnderline.setOrigin({HSModeUnderline.getSize().x / 2.f, 0.f});
    HSModeUnderline.setPosition({400.f, 142.f});
    for (int i = 0; i < VISIBLE_SCORES; i++)
    {
        textHSListRanks[i].setFillColor(colorListText);
        textHSListNames[i].setFillColor(colorListText);
        textHSListScores[i].setFillColor(colorListText);
        textHSListRanks[i].setPosition({260.f, LIST_START_Y + (i * LIST_SPACING)});
        textHSListNames[i].setPosition({290.f, LIST_START_Y + (i * LIST_SPACING)});
    }
    textHSDoneButton.setFillColor(colorDoneText);
    textHSDoneButton.setOrigin({textHSDoneButton.getLocalBounds().size.x / 2.f, textHSDoneButton.getLocalBounds().size.y / 2.f});
    textHSDoneButton.setOutlineThickness(2.f);
    textHSDoneButton.setPosition({400.f, 461.f});
    textHSResetButton.setFillColor(colorResetText);
    textHSResetButton.setOrigin({textHSResetButton.getLocalBounds().size.x / 2.f, textHSResetButton.getLocalBounds().size.y / 2.f});
    textHSResetButton.setPosition({400.f, 557.f});
    updateHighScoreTexts();
}

void UpdateHighscore()
{
    // PlayingSound(false);
    Vector2f mousePos = window.mapPixelToCoords(Mouse::getPosition(window), view);
    bool mouseClicked = false;
    while (const optional event = window.pollEvent())
    {
        if (event->is<Event::Closed>())
            window.close();
        else if (const auto *resizeEvent = event->getIf<Event::Resized>())
        {
            view.setViewport(FloatRect({0.f, 0.f}, {1.f, 1.f}));
            window.setView(view);
        }
        if (const auto *mouseBtn = event->getIf<Event::MouseButtonReleased>())
            if (mouseBtn->button == Mouse::Button::Left)
                mouseClicked = true;
        if (const auto *keyPressed = event->getIf<Event::KeyPressed>())
        {
            if (keyPressed->code == Keyboard::Key::Escape){
                window.setView(view);
                MainMenu();
        }}
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
    auto processArrow = [&](Sprite &s, bool action)
    {
        if (s.getGlobalBounds().contains(mousePos))
        {
            if (isMousePressed)
            {
                s.setTexture(texHSArrowDown, true);
                s.setOrigin({texHSArrowDown.getSize().x / 2.f, texHSArrowDown.getSize().y / 2.f});
            }
            else
            {
                s.setTexture(texHSArrowHover, true);
                s.setOrigin({texHSArrowHover.getSize().x / 2.f, texHSArrowHover.getSize().y / 2.f});
            }
            return action;
        }
        else
        {
            s.setTexture(texHSArrowNormal, true);
            s.setOrigin({texHSArrowNormal.getSize().x / 2.f, texHSArrowNormal.getSize().y / 2.f});
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
        textHSStoryMode.setOrigin({textHSStoryMode.getLocalBounds().size.x / 2.f, 0.f});
        HSModeUnderline.setSize({textHSStoryMode.getLocalBounds().size.x, 2.f});
        HSModeUnderline.setOrigin({HSModeUnderline.getSize().x / 2.f, 0.f});
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

    CheckHoverSound(mousePos, {
    sprHSDonePlank.getGlobalBounds(),
    sprHSResetPlank.getGlobalBounds(),
    sprHSListArrowUp.getGlobalBounds(),
    sprHSListArrowDown.getGlobalBounds(),
    sprHSStoryArrowLeft.getGlobalBounds(),
    sprHSStoryArrowRight.getGlobalBounds()
});
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
        deltaTime = frameClock.restart().asSeconds();
        totaltime += deltaTime;
        BarracudaFishanimation(mCuda);
        QueenTriggerFish(mQueen);
        for (auto &obj : smallfishs)
        {
            obj.shape.setPosition({obj.sprite.getPosition().x, obj.sprite.getPosition().y});
            obj.update(286, 126, deltaTime);
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
    playsound = 1;
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
    creditsText->setOrigin({bounds.position.x + bounds.size.x / 2.f, 0.f});
    creditsText->setPosition({WindowWidth / 2.f, WindowHeight / 2.f - 260.f});
    sprHSDonePlankcredits.setPosition({400.f, 545.f});
    sprHSDonePlankcredits.setOrigin({texHSDoneNormalcredits.getSize().x / 2.f, texHSDoneNormalcredits.getSize().y / 2.f});
    textHSDoneButtoncredits.setPosition({400.f, 536.f});
    textHSDoneButtoncredits.setOutlineThickness(2.f);
    FloatRect doneBounds = textHSDoneButtoncredits.getLocalBounds();
    textHSDoneButtoncredits.setOrigin({doneBounds.size.x / 2.f, doneBounds.size.y / 2.f});
}

bool UpdateCredits()
{
    // PlayingSound(false);
    Vector2f mousePos = window.mapPixelToCoords(Mouse::getPosition(window), view);
    while (const optional event = window.pollEvent())
    {
        if (event->is<Event::Closed>())
        {
            window.close();
            return true;
        }
        if (const auto *keyPressed = event->getIf<Event::KeyPressed>())
        {
            if (keyPressed->code == Keyboard::Key::Escape)
                return true;
        }
        if (const auto *mouseBtn = event->getIf<Event::MouseButtonReleased>())
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
    CheckHoverSound(mousePos, {
    sprHSDonePlankcredits.getGlobalBounds()
});
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
    playsound = 1;
    isLoading = false;
    loadProgress = 0.f;

    if (!bgTexture.loadFromFile("Assets/Select_level/gamemap_bg.jpeg"))
        std::cerr << "Error: Could not load gamemap_bg.jpeg" << std::endl;
    bgSprite.setTexture(bgTexture);
    bgTexture.setSmooth(true);

    if (!signTexture.loadFromFile("Assets/Select_level/gamemap_tittleboard.jpeg"))
        std::cerr << "Error: Could not load gamemap_tittleboard.jpeg" << std::endl;
    signTexture.setSmooth(true);
    signSprite.setTexture(signTexture);
    signSprite.setPosition({10.f, 10.f});

    if (!bannerTexture.loadFromFile("Assets/Select_level/bord.jpeg"))
        std::cerr << "Error: Could not load bord.jpeg" << std::endl;
    bannerTexture.setSmooth(true);
    bannerSprite.setTexture(bannerTexture);
    bannerSprite.setPosition({1.f, 445.f});

    if (!myTexture.loadFromFile("Assets/Select_level/shell_tinybtn124_normal.jpeg"))
        std::cerr << "Error: Could not load shell_tinybtn124_normal.jpeg" << std::endl;
    if (!myHoverTexture.loadFromFile("Assets/Select_level/shell_tinybtn124_high.jpeg"))
        std::cerr << "Error: Could not load shell_tinybtn124_high.jpeg" << std::endl;
    myTexture.setSmooth(true);
    myHoverTexture.setSmooth(true);
    mySprite.setTexture(myTexture);
    mySprite.setPosition({350.f, 565.f});

    if (!pearlTexture.loadFromFile("Assets/Select_level/white.jpeg"))
        std::cerr << "Error: Could not load white.jpeg" << std::endl;
    pearlTexture.setSmooth(true);
    if (!pearlUnlockedTexture.loadFromFile("Assets/Select_level/gold.jpeg"))
        std::cerr << "Error: Could not load gold.jpeg" << std::endl;
    pearlUnlockedTexture.setSmooth(true);
    if (!loadBarTexture.loadFromFile("Assets/Select_level/gamemap_loadbar.jpeg"))
        std::cerr << "Error: Could not load gamemap_loadbar.jpeg" << std::endl;
    loadBarTexture.setSmooth(true);

    if (!font.openFromFile("Assets/Fonts/ARIALNBI.ttf"))
        cout << "Error loading font!\n";
    font.setSmooth(true);

    // init texts
    staticTxt = Text(font, "NEW GAME", 25);
    staticTxt.setPosition({356.f, 493.f});
    menuTxt = Text(font, "menu", 20);
    menuTxt.setPosition({390.f, 570.f});
    levelTxt = Text(font, "choose level", 24);
    centerText(levelTxt);
    loadingtxt = Text(font, "Now Loading...", 24);
    loadingtxt.setPosition({356.f, 493.f});

    // init pearls
    pearls[0] = {&pearlSprite1, 25.f, "Level 1", &level1Unlocked, {215.f, 100.f}, {0.15f, 0.15f}, {215.f, 100.f}, {0.15f, 0.15f}};
    pearls[1] = {&pearlSprite2, 25.f, "Level 2", &level2Unlocked, {248.f, 155.f}, {0.13f, 0.13f}, {236.f, 148.f}, {0.13f, 0.13f}};
    pearls[2] = {&pearlSprite3, 25.f, "Level 3", &level3Unlocked, {232.f, 186.f}, {0.13f, 0.13f}, {225.f, 196.f}, {0.13f, 0.13f}};

    ta_pearls[0] = {&ta_pearlSprite1, 25.f, "Level 1", &ta_level1Unlocked, {220.f, 100.f}, {0.15f, 0.15f}, {220.f, 100.f}, {0.15f, 0.15f}};
    ta_pearls[1] = {&ta_pearlSprite2, 25.f, "Level 2", &ta_level2Unlocked, {248.f, 155.f}, {0.13f, 0.13f}, {236.f, 148.f}, {0.13f, 0.13f}};
    ta_pearls[2] = {&ta_pearlSprite3, 25.f, "Level 3", &ta_level3Unlocked, {232.f, 186.f}, {0.13f, 0.13f}, {225.f, 196.f}, {0.13f, 0.13f}};
}

// center text horizontally on screen
void centerText(Text &text)
{
    FloatRect b = text.getLocalBounds();
    text.setOrigin({b.position.x + b.size.x / 2.f, b.position.y + b.size.y / 2.f});
    text.setPosition({415.f, 545.f});
}

// update pearl states, button hover, load bar progress

void UpdateSelectLevel(float dt)
{
    PearlData *activePearls = isTimeAttackMode ? ta_pearls : pearls;
    // PlayingSound(true);
    Vector2f mf = window.mapPixelToCoords(Mouse::getPosition(window), view);

    bool selHovering = mySprite.getGlobalBounds().contains(mf);
    for (int i = 0; i < 3; ++i)
    {
        if (*activePearls[i].unlocked)
        {
            FloatRect bounds = activePearls[i].sprite->getGlobalBounds();
            Vector2f center = {bounds.position.x + bounds.size.x/2.f,
                               bounds.position.y + bounds.size.y/2.f};
            if (hypot(mf.x - center.x, mf.y - center.y) <= activePearls[i].radius)
                selHovering = true;
        }
    }
    if (selHovering) { if (playsound) { buttonpressedsound.play(); playsound = 0; } }
    else playsound = 1;
    
    levelTxt.setString("choose level");
    centerText(levelTxt);


    static bool pearlClicked = false;

    // set texture/position per pearl & show name on hover
    for (int i = 0; i < 3; ++i)
    {
        auto &p = activePearls[i];

        p.sprite->setPosition(p.unlockedPos);
        p.sprite->setScale(p.unlockedScale);

        
        if (*p.unlocked)
        {
            p.sprite->setTexture(pearlUnlockedTexture);
        }
        else
        {
            p.sprite->setTexture(pearlTexture);
        }
        

        FloatRect bounds = p.sprite->getGlobalBounds();
        Vector2f center = {bounds.position.x + bounds.size.x / 2.f, bounds.position.y + bounds.size.y / 2.f};
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
        else
            mySprite.setTexture(myTexture);
    }
    else
        mySprite.setPosition({-1000.f, -1000.f});

    // advance load bar; on complete → transition to game
    if (isLoading)
    {
        
        if (mainmenumusic.getStatus() == SoundSource::Status::Playing)
        {
            mainmenumusic.stop();
        }

        if (loadingmusic.getStatus() != SoundSource::Status::Playing)
        {
            loadingmusic.play();
            loadingmusic.setLooping(true);
        }

        mySprite.setPosition({-3000.f, -3000.f});
        menuTxt.setPosition({-3000.f, -3000.f});
        loadProgress += barSpeed * dt;
        if (loadProgress >= barWidth)
        {
            loadProgress = barWidth;
            DrawSelectLevel();
            fadeAlpha = 0.f;
            fadeClock.restart();
            while (fadeAlpha < 255.f)
            {
                fadeAlpha = std::min(255.f, fadeClock.getElapsedTime().asSeconds() * 800.f);
                fadeRect.setFillColor(Color(0, 0, 0, (uint8_t)fadeAlpha));
                window.setView(view);
                DrawSelectLevel();
                window.draw(fadeRect);
                window.display();
            }

            loadingdone = true;

            int clickedPearlIndex = selectedLevel - 1;

            if (loadingdone && clickedPearlIndex != -1)
            {
               
                switch (clickedPearlIndex)
                {
                case 0: // Level 1
                    currentLevelSettings.fishToGrowToLevel2 = 20;
                    currentLevelSettings.fishToGrowToLevel3 = 40;
                    currentLevelSettings.fishToWin = 60; 
                    timeAttackDuration = 60.0f;          
                    break;
                case 1: // Level 2
                    currentLevelSettings.fishToGrowToLevel2 = 25;
                    currentLevelSettings.fishToGrowToLevel3 = 50;
                    currentLevelSettings.fishToWin = 75; 
                    timeAttackDuration = 45.0f;          
                    break;
                case 2: // Level 3
                    currentLevelSettings.fishToGrowToLevel2 = 35;
                    currentLevelSettings.fishToGrowToLevel3 = 70;
                    currentLevelSettings.fishToWin = 105; 
                    timeAttackDuration = 30.0f;           
                    break;
                }

                
                PearlData *targetPearls = isTimeAttackMode ? ta_pearls : pearls;

                if (*targetPearls[clickedPearlIndex].unlocked)
                {
                    GameScreen(clickedPearlIndex + 1);

                    view.setSize({WindowWidth, WindowHeight});
                    view.setCenter({WindowWidth / 2.f, WindowHeight / 2.f});

                    isLoading = false;
                    loadingdone = false;
                    mySprite.setPosition({ 350.f, 565.f });
                    menuTxt.setPosition({ 390.f, 570.f }); 

                    
                    pearlClicked = false;

                    if (goToMainMenuFromLevel)
                    {
                        return;
                    }
                }
            }
           
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
    if (!isTimeAttackMode)
    {
        window.draw(pearlSprite1);
        window.draw(pearlSprite2);
        window.draw(pearlSprite3);
    }
    else
    {
        window.draw(ta_pearlSprite1);
        window.draw(ta_pearlSprite2);
        window.draw(ta_pearlSprite3);
    }

    window.draw(mySprite);
    window.draw(menuTxt);
    window.draw(levelTxt);

    // load bar OR static "NEW GAME" text
    if (isLoading)
    {
        Sprite bar(loadBarTexture);
        bar.setPosition({barX, barY});
        float sx = barWidth / (float)loadBarTexture.getSize().x;
        float sy = barHeight / (float)loadBarTexture.getSize().y;
        bar.setScale({sx, sy});
        bar.setTextureRect(IntRect({0, 0}, {(int)(loadProgress / sx), (int)loadBarTexture.getSize().y}));
        window.draw(bar);
        window.draw(loadingtxt);
    }
    else
        window.draw(staticTxt);

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
            if (event->is<Event::Closed>())
                window.close();
            else if (const auto *resizeEvent = event->getIf<Event::Resized>())
            {
                view.setViewport(FloatRect({0.f, 0.f}, {1.f, 1.f}));
                window.setView(view);
            }

            // click: back button or unlocked pearl
            if (auto mouseEvent = event->getIf<Event::MouseButtonReleased>())
                if (mouseEvent->button == Mouse::Button::Left)
                {
                    if (isLoading)
                        continue;
                    Vector2f mf = window.mapPixelToCoords(Mouse::getPosition(window), view);
                    if (mySprite.getGlobalBounds().contains(mf))
                        return;

                    PearlData *activePearls = isTimeAttackMode ? ta_pearls : pearls;
                    for (int i = 0; i < 3; ++i)
                    {
                        if (*activePearls[i].unlocked)
                        {
                            FloatRect bounds = activePearls[i].sprite->getGlobalBounds();
                            Vector2f center = {bounds.position.x + bounds.size.x / 2.f,
                                               bounds.position.y + bounds.size.y / 2.f};
                            if (hypot(mf.x - center.x, mf.y - center.y) <= activePearls[i].radius)
                            {

                                isLoading = true;
                                loadProgress = 0.f;
                                selectedLevel = i + 1;
                                pearlClicked = true;
                                if (!isTimeAttackMode)
                                {

                                    if (selectedLevel != lastWonStoryLevel + 1)
                                        storyCarryScore = 0;
                                }
                                else
                                {
                                    if (selectedLevel != lastWonTALevel + 1)
                                        taCarryScore = 0;
                                }

                            }
                        }
                    }
                }
        if (const auto *keyPressed = event->getIf<Event::KeyPressed>())
        {
            if (keyPressed->code == Keyboard::Key::Escape){
                window.setView(view);
                MainMenu();
        }
    }
        }
        window.setView(view);
        UpdateSelectLevel(dt);
        if (goToMainMenuFromLevel)
        {
            goToMainMenuFromLevel = false; // Reset flag for next time
            return;                        
        }
        DrawSelectLevel();
    }
}

// ==========================================
//          ===== Game Screen =====
// ==========================================

// load bg, buttons, quit popup & level-specific assets
void StartGameScreen(int level)
{
    playsound = 1;
    selectedLevel = level;
    gameScreenActive = true;
    window.setView(view);

    // background
    if (!gameScreenBgTexture.loadFromFile(
            "Assets/GameScreen/shell_stageinfo.jpg"))
        cout << "Error: shell_stageinfo.jpg\n";
    gameScreenBgTexture.setSmooth(true);
    gameScreenBgSprite = sf::Sprite(gameScreenBgTexture);
    sf::Vector2u bgSize = gameScreenBgTexture.getSize();
    gameScreenBgSprite.setScale({WindowWidth / (float)bgSize.x, WindowHeight / (float)bgSize.y});
    gameScreenBgSprite.setPosition({0.f, 0.f});

    // options / quit / continue buttons
    struct BtnInfo
    {
        const char *n;
        const char *h;
        float x, y, sc, hsc, hw, hh;
    };
    BtnInfo btnInfos[3] = {
        {"Assets/GameScreen/option1.png",
         "Assets/GameScreen/option2.png",
         180.f, 560.f, 1 / 10.f, 1 / 10.f, 0.f, 0.f},
        {"Assets/GameScreen/quit1.png",
         "Assets/GameScreen/quit2.png",
         630.f, 560.f, 2 / 10.f, 2 / 10.f, 0.f, 0.f},
        {"Assets/GameScreen/continue1.png",
         "Assets/GameScreen/continue_large-removebg-preview.png",
         400.f, 540.f, 1.5f / 10.f, 3.5f / 10.f, 80.f, 30.f},
    };
    for (int i = 0; i < 3; i++)
    {
        if (!gsBtns[i].normalTex.loadFromFile(btnInfos[i].n))
            cout << "Error btn normal " << i << "\n";
        if (!gsBtns[i].hoverTex.loadFromFile(btnInfos[i].h))
            cout << "Error btn hover " << i << "\n";
        gsBtns[i].normalTex.setSmooth(true);
        gsBtns[i].hoverTex.setSmooth(true);
        gsBtns[i].x = btnInfos[i].x;
        gsBtns[i].y = btnInfos[i].y;
        gsBtns[i].scale = btnInfos[i].sc;
        gsBtns[i].hoverScale = btnInfos[i].hsc;
        gsBtns[i].hoverHalfW = btnInfos[i].hw;
        gsBtns[i].hoverHalfH = btnInfos[i].hh;
        gsBtns[i].sprite = sf::Sprite(gsBtns[i].normalTex);
        gsBtns[i].sprite->setOrigin({gsBtns[i].normalTex.getSize().x / 2.f,
                                     gsBtns[i].normalTex.getSize().y / 2.f});
        gsBtns[i].sprite->setPosition({gsBtns[i].x, gsBtns[i].y});
        gsBtns[i].sprite->setScale({gsBtns[i].scale, gsBtns[i].scale});
    }

    // quit popup - bg, title, texts
    showQuitPopup = false;
    if (!quitPopup.bgTex.loadFromFile(
            "Assets/GameScreen/shell_shared_popupbg_medium-removebg-preview (2).png"))
        ;
    quitPopup.bgTex.setSmooth(true);
    quitPopup.bgSprite = sf::Sprite(quitPopup.bgTex);
    quitPopup.bgSprite->setOrigin({quitPopup.bgTex.getSize().x / 2.f, quitPopup.bgTex.getSize().y / 2.f});
    quitPopup.bgSprite->setPosition({QUIT_POPUP_X, QUIT_POPUP_Y});
    quitPopup.bgSprite -> setScale({1.,1.});

    if (!quitPopup.titleTex.loadFromFile(
            "Assets/GameScreen/New Project (1).png"))
        ;
    quitPopup.titleTex.setSmooth(true);
    quitPopup.titleSprite = sf::Sprite(quitPopup.titleTex);
    quitPopup.titleSprite->setOrigin({quitPopup.titleTex.getSize().x / 2.f, quitPopup.titleTex.getSize().y / 2.f});
    quitPopup.titleSprite->setPosition({QUIT_POPUP_X, 180.f});
    quitPopup.titleSprite->setScale({0.5f, 0.5f});

    quitPopupLine1 = Text(font, "Are you sure you want to quit?", 22);
    quitPopupLine1.setFillColor(Color::White);
    {
        FloatRect b = quitPopupLine1.getLocalBounds();
        quitPopupLine1.setOrigin({b.size.x / 2.f, b.size.y / 2.f});
        quitPopupLine1.setPosition({QUIT_POPUP_X, 230.f});
    }

    quitPopupLine2 = Text(font, "(Select quit option)", 18);
    quitPopupLine2.setFillColor(Color(200, 230, 200));
    {
        FloatRect b = quitPopupLine2.getLocalBounds();
        quitPopupLine2.setOrigin({b.size.x / 2.f, b.size.y / 2.f});
        quitPopupLine2.setPosition({QUIT_POPUP_X, 250.f});
    }

    // quit popup buttons
    struct PopupBtnInfo
    {
        const char *n;
        const char *h;
        float y;
    };
    PopupBtnInfo pbInfos[3] = {
        {"Assets/GameScreen/Screenshot_2026-04-25_222549-removebg-preview.png",
         "Assets/GameScreen/Screenshot_2026-04-25_164334-removebg-preview.png", QUIT_BTN1_Y},
        {"Assets/GameScreen/Screenshot_2026-04-25_223151-removebg-preview.png",
         "Assets/GameScreen/Screenshot_2026-04-25_230020-removebg-preview.png", QUIT_BTN2_Y},
        {"Assets/GameScreen/Screenshot_2026-04-25_223327-removebg-preview.png",
         "Assets/GameScreen/Screenshot_2026-04-25_230106-removebg-preview.png", QUIT_BTN3_Y},
    };
    for (int i = 0; i < 3; i++)
    {
        if (!quitPopup.btns[i].normalTex.loadFromFile(pbInfos[i].n))
            cout << "Error popup btn normal " << i << "\n";
        if (!quitPopup.btns[i].hoverTex.loadFromFile(pbInfos[i].h))
            cout << "Error popup btn hover " << i << "\n";
        quitPopup.btns[i].sprite = sf::Sprite(quitPopup.btns[i].normalTex);
        quitPopup.btns[i].sprite->setOrigin({quitPopup.btns[i].normalTex.getSize().x / 2.f,
                                             quitPopup.btns[i].normalTex.getSize().y / 2.f});
        quitPopup.btns[i].sprite->setPosition({QUIT_POPUP_X, pbInfos[i].y});
        quitPopup.btns[i].sprite->setScale({QUIT_BTN_SCALE, QUIT_BTN_SCALE});
    }

    // level-specific images & texts
    int idx = level - 1;
    LevelData &ld = levels[idx];
    if (!ld.font.openFromFile("Assets/Fonts/trebuc.ttf"))
        ;
    ld.font.setSmooth(true);

    if (level == 1)
    {
        ld.imageCount = 4;
        struct ImgInfo
        {
            const char *path;
            float x, y, sc;
        };
        ImgInfo imgs[4] = {
            {"Assets/GameScreen/menu_butterflyfish-removebg-preview.png", 580.f, 230.f, 0.5f},
            {"Assets/GameScreen/Screenshot_2026-04-24_222635-removebg-preview.png", 300.f, 403.f, 0.9f},
            {"Assets/GameScreen/Screenshot_2026-04-24_222758-removebg-preview.png", 410.f, 405.f, 0.2f},
            {"Assets/GameScreen/hudimage-removebg.png", 480.f, 415.f, 0.6f},
        };
        for (int i = 0; i < 4; i++)
        {
            if (!ld.images[i].tex.loadFromFile(imgs[i].path))
                cout << "Error lvl1 img " << i << "\n";
            ld.images[i].tex.setSmooth(true);
            ld.images[i].x = imgs[i].x;
            ld.images[i].y = imgs[i].y;
            ld.images[i].scale = imgs[i].sc;
            ld.images[i].sprite = sf::Sprite(ld.images[i].tex);
            ld.images[i].sprite->setOrigin({ld.images[i].tex.getSize().x / 2.f, ld.images[i].tex.getSize().y / 2.f});
            ld.images[i].sprite->setPosition({imgs[i].x, imgs[i].y});
            ld.images[i].sprite->setScale({imgs[i].sc, imgs[i].sc});
        }

        ld.textCount = 6;
        struct TxtInfo
        {
            const char *str;
            int sz;
            Color col;
            float x, y;
            bool c;
        };
        TxtInfo txts[6] = {
            {"1. Level 1", 26, Color(255, 220, 50), 415.f, 55.f, true},
            {"Time to get your fins wet!", 16, Color(255, 255, 255), 415.f, 88.f, true},
            {"Meet Boris the Butterfly Fish. Boris lives in the\nwarm waters of the Sandy Shoal - a beautiful\narea off the Frenzy Coast. Enjoy the sights, but\ndon't get too comfortable... A fish still needs his\nlunch, and it's a fish-eat-fish world out there!",
             14, Color(240, 240, 240), 150.f, 145.f, false},
            {"How to play:", 17, Color(240, 240, 240), 145.f, 250.f, false},
            {"- Use your mouse to control Boris.\n- Eat fish that are smaller than you.\n- Avoid anything that's larger than you.\n- Eat enough fish and you'll grow bigger!",
             13, Color(240, 240, 240), 160.f, 280.f, false},
            {"Warm up those chompers and have some fun!", 15, Color(240, 240, 240), 390.f, 480.f, true},
        };
        for (int i = 0; i < 6; i++)
        {
            ld.texts[i].text.emplace(ld.font);
            ld.texts[i].text->setString(txts[i].str);
            ld.texts[i].text->setCharacterSize(txts[i].sz);
            ld.texts[i].text->setFillColor(txts[i].col);
            ld.texts[i].text->setOutlineColor(Color(20, 20, 20));
            ld.texts[i].text->setOutlineThickness(i == 0 ? 2.f : 1.f);
            if (txts[i].c)
            {
                FloatRect b = ld.texts[i].text->getLocalBounds();
                ld.texts[i].text->setOrigin({b.size.x / 2.f, b.size.y / 2.f});
            }
            ld.texts[i].text->setPosition({txts[i].x, txts[i].y});
        }
    }
    else if (level == 2)
    {
        ld.imageCount = 4;
        struct ImgInfo
        {
            const char *path;
            float x, y, sc;
        };
        ImgInfo imgs[4] = {
            {"Assets/GameScreen/starbubble1.png", 580.f, 200.f, 0.9f},
            {"Assets/GameScreen/sparkletrail.png", 580.f, 240.f, 0.9f},
            {"Assets/GameScreen/scr_mouseleftclickarrow-removebg-preview.png", 320.f, 405.f, 0.9f},
            {"Assets/GameScreen/Screenshot_2026-04-24_223502-removebg-preview.png", 440.f, 415.f, 0.6f},
        };
        for (int i = 0; i < 4; i++)
        {
            if (!ld.images[i].tex.loadFromFile(imgs[i].path))
                cout << "Error lvl2 img " << i << "\n";
            ld.images[i].tex.setSmooth(true);
            ld.images[i].sprite = sf::Sprite(ld.images[i].tex);
            ld.images[i].sprite->setOrigin({ld.images[i].tex.getSize().x / 2.f, ld.images[i].tex.getSize().y / 2.f});
            ld.images[i].sprite->setPosition({imgs[i].x, imgs[i].y});
            ld.images[i].sprite->setScale({imgs[i].sc, imgs[i].sc});
        }

        ld.textCount = 7;
        struct TxtInfo
        {
            const char *str;
            int sz;
            Color col;
            float x, y;
            bool c;
        };
        TxtInfo txts[7] = {
            {"2. Level 2", 26, Color(255, 220, 50), 415.f, 55.f, true},
            {"A Tasty Treat", 16, Color(255, 255, 255), 415.f, 88.f, true},
            {"Watch for Starfish!", 17, Color(240, 240, 240), 150.f, 145.f, false},
            {"Tasty STARFISH have been seen floating around\nthese parts. Grab a starfish bubble for some\nextra points. They won't help Boris get any\nbigger, but they're sure to grow your score!",
             14, Color(240, 240, 240), 150.f, 175.f, false},
            {"How to Dash:", 17, Color(240, 240, 240), 150.f, 270.f, false},
            {"Tap the left mouse button for a short burst of speed.\nIt doesn't last long, but it might help you out\nof a tough spot.",
             14, Color(240, 240, 240), 150.f, 300.f, false},
            {"Stay alert, and watch out for sharks!", 15, Color(240, 240, 240), 390.f, 480.f, true},
        };
        for (int i = 0; i < 7; i++)
        {
            ld.texts[i].text.emplace(ld.font);
            ld.texts[i].text->setString(txts[i].str);
            ld.texts[i].text->setCharacterSize(txts[i].sz);
            ld.texts[i].text->setFillColor(txts[i].col);
            ld.texts[i].text->setOutlineColor(Color(20, 20, 20));
            ld.texts[i].text->setOutlineThickness(i == 0 ? 2.f : 1.f);
            if (txts[i].c)
            {
                FloatRect b = ld.texts[i].text->getLocalBounds();
                ld.texts[i].text->setOrigin({b.size.x / 2.f, b.size.y / 2.f});
            }
            ld.texts[i].text->setPosition({txts[i].x, txts[i].y});
        }
    }
    else if (level == 3)
    {
        ld.imageCount = 3;
        struct ImgInfo
        {
            const char *path;
            float x, y, sc;
        };
        ImgInfo imgs[3] = {
            {"Assets/GameScreen/ffbubble1.png", 250.f, 350.f, 1.0f},
            {"Assets/GameScreen/sparkletrail.png", 250.f, 400.f, 0.8f},
            {"Assets/GameScreen/scr_frenzymeter-removebg-preview.png", 400.f, 280.f, 0.8f},
        };
        for (int i = 0; i < 3; i++)
        {
            if (!ld.images[i].tex.loadFromFile(imgs[i].path))
                cout << "Error lvl3 img " << i << "\n";
            ld.images[i].tex.setSmooth(true);
            ld.images[i].sprite = sf::Sprite(ld.images[i].tex);
            ld.images[i].sprite->setOrigin({ld.images[i].tex.getSize().x / 2.f, ld.images[i].tex.getSize().y / 2.f});
            ld.images[i].sprite->setPosition({imgs[i].x, imgs[i].y});
            ld.images[i].sprite->setScale({imgs[i].sc, imgs[i].sc});
        }

        ld.textCount = 7;
        struct TxtInfo
        {
            const char *str;
            int sz;
            Color col;
            float x, y;
            bool c;
        };
        TxtInfo txts[7] = {
            {"3. Level 3", 26, Color(255, 220, 50), 415.f, 55.f, true},
            {"Now with even more Frenzy!", 16, Color(255, 255, 255), 415.f, 88.f, true},
            {"Frenzy Time!", 17, Color(240, 240, 240), 150.f, 145.f, false},
            {"Now that you're warmed up, it's time to get your Frenzy on.\nEat fish quickly to fill up the FRENZY METER. Each time you\nfill the meter, your Frenzy Multiplier increases and everything\nyou eat is worth more points! Try to reach Mega Frenzy for a 6x bonus!",
             14, Color(240, 240, 240), 150.f, 175.f, false},
            {"Frenzy Boost", 17, Color(240, 240, 240), 320.f, 320.f, false},
            {"Look out for special Frenzy Boost power-ups.\nEat a Frenzy Booster to fill up the Frenzy\nMeter in a single chomp!",
             14, Color(240, 240, 240), 320.f, 345.f, false},
            {"You have to keep eating to keep the Frenzy Meter full!", 15, Color(240, 240, 240), 390.f, 450.f, true},
        };
        for (int i = 0; i < 7; i++)
        {
            ld.texts[i].text.emplace(ld.font);
            ld.texts[i].text->setString(txts[i].str);
            ld.texts[i].text->setCharacterSize(txts[i].sz);
            ld.texts[i].text->setFillColor(txts[i].col);
            ld.texts[i].text->setOutlineColor(Color(20, 20, 20));
            ld.texts[i].text->setOutlineThickness(i == 0 ? 2.f : 1.f);
            if (txts[i].c)
            {
                FloatRect b = ld.texts[i].text->getLocalBounds();
                ld.texts[i].text->setOrigin({b.size.x / 2.f, b.size.y / 2.f});
            }
            ld.texts[i].text->setPosition({txts[i].x, txts[i].y});
        }
    }
}

// button hover states each frame
void UpdateGameScreen()
{
    Vector2f mousePos = window.mapPixelToCoords(Mouse::getPosition(window));

    for (int i = 0; i < 2; i++) 
    {
        if (!gsBtns[i].sprite) continue;
        if (gsBtns[i].sprite->getGlobalBounds().contains(mousePos))
        {
            gsBtns[i].sprite->setTexture(gsBtns[i].hoverTex);
            gsBtns[i].sprite->setOrigin({ gsBtns[i].hoverTex.getSize().x / 2.f,
                                           gsBtns[i].hoverTex.getSize().y / 2.f });
        }
        else
        {
            gsBtns[i].sprite->setTexture(gsBtns[i].normalTex);
            gsBtns[i].sprite->setOrigin({ gsBtns[i].normalTex.getSize().x / 2.f,
                                           gsBtns[i].normalTex.getSize().y / 2.f });
        }
    }

    if (gsBtns[2].sprite)
    {
        bool hovered = mousePos.x >= gsBtns[2].x - gsBtns[2].hoverHalfW &&
                       mousePos.x <= gsBtns[2].x + gsBtns[2].hoverHalfW &&
                       mousePos.y >= gsBtns[2].y - gsBtns[2].hoverHalfH &&
                       mousePos.y <= gsBtns[2].y + gsBtns[2].hoverHalfH;
        if (hovered)
        {
            gsBtns[2].sprite->setTexture(gsBtns[2].hoverTex);
            gsBtns[2].sprite->setOrigin({ gsBtns[2].hoverTex.getSize().x / 2.f,
                                           gsBtns[2].hoverTex.getSize().y / 2.f });
            gsBtns[2].sprite->setScale({ gsBtns[2].hoverScale, gsBtns[2].hoverScale });
        }
        else
        {
            gsBtns[2].sprite->setTexture(gsBtns[2].normalTex);
            gsBtns[2].sprite->setOrigin({ gsBtns[2].normalTex.getSize().x / 2.f,
                                           gsBtns[2].normalTex.getSize().y / 2.f });
            gsBtns[2].sprite->setScale({ gsBtns[2].scale, gsBtns[2].scale });
        }
    }

    if (showQuitPopup)
    {
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
    bool endHovering = false;
    for (int i = 0; i < 3; i++)
        if (gsBtns[i].sprite && gsBtns[i].sprite->getGlobalBounds().contains(mousePos))
            endHovering = true;
    if (showQuitPopup)
        for (int i = 0; i < 3; i++)
            if (quitPopup.btns[i].sprite && quitPopup.btns[i].sprite->getGlobalBounds().contains(mousePos))
                endHovering = true;
    if (endHovering) { if (playsound) { buttonpressedsound.play(); playsound = 0; } }
    else playsound = 1;
}

// draw buttons + quit popup OR level images/texts
void DrawGameScreen()
{
    window.setView(view);
    window.clear(Color::Black);
    window.draw(gameScreenBgSprite);

    for (int i = 0; i < 3; i++)
        if (gsBtns[i].sprite)
            window.draw(*gsBtns[i].sprite);

    if (showQuitPopup)
    {
        if (quitPopup.bgSprite)
            window.draw(*quitPopup.bgSprite);
        if (quitPopup.titleSprite)
            window.draw(*quitPopup.titleSprite);
        for (int i = 0; i < 3; i++)
            if (quitPopup.btns[i].sprite)
                window.draw(*quitPopup.btns[i].sprite);
        window.draw(quitPopupLine1);
        window.draw(quitPopupLine2);
    }
    else
    {
        LevelData &ld = levels[selectedLevel - 1];
        for (int i = 0; i < ld.imageCount; i++)
            if (ld.images[i].sprite)
                window.draw(*ld.images[i].sprite);
        for (int i = 0; i < ld.textCount; i++)
            if (ld.texts[i].text)
                window.draw(*ld.texts[i].text);
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
        window.setView(view); 
        while (const optional event = window.pollEvent())
        {
            if (event->is<Event::Closed>())
                window.close();
            else if (const auto *keyPressed = event->getIf<Event::KeyPressed>())
            {
                if (keyPressed->code == Keyboard::Key::Escape)
                {
                    if (showQuitPopup)
                    {

                        showQuitPopup = false;
                    }
                    else
                    {

                        if (loadingmusic.getStatus() == SoundSource::Status::Playing)
                            loadingmusic.stop();
                        if (levelsound.getStatus() == SoundSource::Status::Playing)
                            levelsound.stop();
                        if (WaveSound.getStatus() == SoundSource::Status::Playing)
                            WaveSound.stop();    

                        gameScreenActive = false;
                        goToMainMenuFromLevel = true;
                        
                        Select_level();
                        mainmenumusic.play();
                        
                        return; 
                    }
                }
            }

            // left click → buttons / quit popup
            if (const auto *mouseBtn = event->getIf<Event::MouseButtonReleased>())
            {
                if (mouseBtn->button == Mouse::Button::Left)
                {
                    Vector2f mousePos = window.mapPixelToCoords(Mouse::getPosition(window), view);
                    if (showQuitPopup)
                    {
                        if (quitPopup.btns[0].sprite && quitPopup.btns[0].sprite->getGlobalBounds().contains(mousePos))
                        {
                            loadingmusic.stop();
                            showQuitPopup = false;
                            gameScreenActive = false;
                            QuitLevelLoadingScreen();
                            MainMenu();
                            return;
                        }
                        if (quitPopup.btns[1].sprite && quitPopup.btns[1].sprite->getGlobalBounds().contains(mousePos))
                        {
                            loadingmusic.stop();
                            showQuitPopup = false;
                            gameScreenActive = false;
                            QuitLevelLoadingScreen(); 
                            MainMenu();
                            return;
                        }
                        if (quitPopup.btns[2].sprite && quitPopup.btns[2].sprite->getGlobalBounds().contains(mousePos))
                            showQuitPopup = false;
                    }
                    else
                    {
                        if (gsBtns[0].sprite && gsBtns[0].sprite->getGlobalBounds().contains(mousePos))
                        {
                            gameScreenActive = false;
                            g_optionsFromPause = true;  
                            OptionsMenu();
                            g_optionsFromPause = false; 
                            gameScreenActive = true;
                        }
                        if (gsBtns[1].sprite && gsBtns[1].sprite->getGlobalBounds().contains(mousePos))
                            showQuitPopup = true;

                        
                        if (gsBtns[2].sprite)
                        {
                            bool hovered = mousePos.x >= gsBtns[2].x - gsBtns[2].hoverHalfW &&
                                           mousePos.x <= gsBtns[2].x + gsBtns[2].hoverHalfW &&
                                           mousePos.y >= gsBtns[2].y - gsBtns[2].hoverHalfH &&
                                           mousePos.y <= gsBtns[2].y + gsBtns[2].hoverHalfH;
                            if (hovered)
                            {
                                // 1. Stop loading music
                                loadingmusic.stop();

                                // 2. Play level music
                                levelsound.play();
                                levelsound.setLooping(true);

                               
                                if (currentGamemode == TIMEATTACK)
                                {
                                    Timeattacklevel();
                                }
                                else
                                {
                                    bglevel();
                                }
                                gameScreenActive = false;
                                return; // Return safely when the level finishes
                            }
                        }
                    }
                }
            }
        }
        UpdateGameScreen();
        DrawGameScreen();
    }
}

// ==========================================
//          Power-Up System
// ==========================================

void StartPowerUps()
{
    for (int i = 0; i < MAX_POWERUPS; i++)
    {
        powerUps[i].active = false;
        powerUps[i].isPopping = false;
        powerUps[i].popTimer = 0.f;
        powerUps[i].scaleX = 1.f;
        powerUps[i].scaleY = 1.f;
    }
    powerUpSpawnTimer = 0.f;
    speedBoostActive = false;
    speedBoostTimer = 0.f;

    if (!powerUpTexLoaded)
    {
        if (!texPowerUpTime.loadFromFile("Assets/bouns/timebubble1.png"))
            cout << "PowerUp: missing time texture\n";
        if (!texPowerUpStar.loadFromFile("Assets/bouns/starbubble1.png"))
            cout << "PowerUp: missing star texture\n";
        if (!texPowerUpSpeed.loadFromFile("Assets/bouns/speedbubble1.png"))
            cout << "PowerUp: missing speed texture\n";
        if (!texPowerUpShrink.loadFromFile("Assets/bouns/shrinkbubble1.png"))
            cout << "PowerUp: missing shrink texture\n";

        texPowerUpShrink.setSmooth(true);
        texPowerUpTime.setSmooth(true);
        texPowerUpStar.setSmooth(true);
        texPowerUpSpeed.setSmooth(true);
        powerUpTexLoaded = true;
    }
}

void UpdatePowerUps(float dt)
{

    if (!stopSpawning)
    {
        powerUpSpawnTimer += dt;
        if (powerUpSpawnTimer >= powerUpSpawnInterval)
        {
            powerUpSpawnTimer = 0.f;

            for (int i = 0; i < MAX_POWERUPS; i++)
            {
                if (!powerUps[i].active)
                {
                    powerUps[i].active = true;
                    powerUps[i].isPopping = false;
                    powerUps[i].popTimer = 0.f;
                    powerUps[i].scaleX = 1.f;
                    powerUps[i].scaleY = 1.f;

                    // Random X across the level, spawn just above the visible top
                    powerUps[i].x = getRandom(60.f, LevelWidth - 60.f);
                    powerUps[i].y = view.getCenter().y - view.getSize().y / 2.f - 60.f;
                    powerUps[i].vy = getRandom(70.f, 130.f); // fall speed

                    // Pick type
                    if (currentGamemode == TIMEATTACK)
                    {
                        powerUps[i].type = rand() % 3;
                    }
                    else if (gameLEVEL == 1)
                    {
                        powerUps[i].type = 1;
                    }
                    else if (gameLEVEL == 2)
                    {
                        powerUps[i].type = 1 + rand() % 2;
                    }
                    else if (gameLEVEL == 3)
                    {
                        int roll = rand() % 2;
                        powerUps[i].type = (roll == 0) ? 2 : 3;
                    }

                    break;
                }
            }
        }
    }

    // --- Update each bubble ---
    Vector2f playerPos = sprPlayerall.getPosition();

    for (int i = 0; i < MAX_POWERUPS; i++)
    {
        if (!powerUps[i].active)
            continue;

        PowerUp &p = powerUps[i];


        // Fall downward
        p.y += p.vy * dt;

        // Remove if it fell below the level floor
        if (p.y > LevelHeight + 80.f)
        {
            p.active = false;
            continue;
        }

        if (p.isPopping)
        {
            p.popTimer += dt;
            const float FRAME0_DURATION = 0.15f; 
            const float FRAME1_DURATION = 0.15f; 
            const float POP_DURATION = FRAME0_DURATION + FRAME1_DURATION;

            if (p.popTimer >= POP_DURATION)
            {
                p.active = false;
                p.isPopping = false;
                continue;
            }

        }

        // --- Collision with player ---
        if (!isPlayerDead)
        {
            float dx = playerPos.x - p.x;
            float dy = playerPos.y - p.y;
            float dist = sqrt(dx * dx + dy * dy);

            if (dist < 45.f) // collection radius
            {
                p.active = false;

                // Apply effect
                if (p.type == 0) // Time
                {
                    // +5 seconds (time attack)
                    if (currentGamemode == TIMEATTACK)
                        remainingTime += 5.f;
                    eatSound.play();
                    createScorePopup(p.x, p.y - 20.f, "+5"); 
                }
                else if (p.type == 1) // Star
                {
                    int pts = 100 * multiplier;
                    score += pts;
                    
                    eatSound.play();
                    createScorePopup(p.x, p.y - 20.f, pts); 
                    
                    starsEatenCount++; 
                    
                    // Eat animation on player
                    if (currentState == TURN)
                        pendingEat = true;
                    else
                    {
                        currentState = EAT;
                        currentFrame = 0;
                        timer = 0.f;
                    }
                }
                else if (p.type == 2) // Speed
                {
                    // Speed boost
                    speedBoostActive = true;
                    speedBoostTimer = 0.f;
                    eatSound.play();
                    createScorePopup(p.x, p.y - 20.f, "SPEED"); 
                }
                else if (p.type == 3) // Shrink
                {
                    // Shrink logic...
                    const float shrinkScale = 0.2f; 

                    for (int j = 0; j < MAX_SMALL_FISH; j++)
                        if (smallFishes[j].active && smallFishes[j].sprite)
                        {
                            sf::Vector2f s = smallFishes[j].sprite->getScale();
                            smallFishes[j].sprite->setScale({(s.x < 0 ? -shrinkScale : shrinkScale),
                                                             shrinkScale});
                        }

                    for (int j = 0; j < MAX_MEDIUM_FISH; j++)
                        if (mediumFishes[j].active && mediumFishes[j].sprite)
                        {
                            sf::Vector2f s = mediumFishes[j].sprite->getScale();
                            mediumFishes[j].sprite->setScale({(s.x < 0 ? -shrinkScale : shrinkScale),
                                                              shrinkScale});
                            mediumFishes[j].isFleeing = false;
                        }

                    for (int j = 0; j < MAX_LARGE_FISH; j++)
                        if (largeFishes[j].active && largeFishes[j].sprite)
                        {
                            sf::Vector2f s = largeFishes[j].sprite->getScale();
                            largeFishes[j].sprite->setScale({(s.x < 0 ? -shrinkScale : shrinkScale),
                                                             shrinkScale});
                            largeFishes[j].isFleeing = false;
                        }

                    eatSound.play();
                    createScorePopup(p.x, p.y - 20.f, "SHRINK"); 
                }

                // Burst bubbles at collection point
                for (int k = 0; k < 6; k++)
                    StartGameBubble(p.x + getRandom(-15.f, 15.f),
                                    p.y + getRandom(-15.f, 15.f), true);
            }
        }
    }

    // --- Speed boost countdown ---
    if (speedBoostActive)
    {
        speedBoostTimer += dt;
        if (speedBoostTimer >= SPEED_BOOST_DURATION)
        {
            speedBoostActive = false;
            speedBoostTimer = 0.f;
        }
    }
}

void DrawPowerUps()
{
    Texture *texPtrs[4] = {&texPowerUpTime, &texPowerUpStar, &texPowerUpSpeed, &texPowerUpShrink};

    for (int i = 0; i < MAX_POWERUPS; i++)
    {
        if (!powerUps[i].active)
            continue;

        PowerUp &p = powerUps[i];

        Sprite bub(*texPtrs[p.type]);
        bub.setOrigin({texPtrs[p.type]->getSize().x / 2.f,
                       texPtrs[p.type]->getSize().y / 2.f});
        bub.setPosition({p.x, p.y});
        bub.setScale({0.8f, 0.8f});
        window.draw(bub);
    }
}

// bglevel and player logic

void bglevel()
{
    gameScreenActive = false;
    gameLEVEL = selectedLevel;
    isLevelRunning = true;
    goToMainMenuFromLevel = false;
    Startbglevel();
    StartLevelHud();
    Startmovingplayer();
    
    view.setSize({800.f, 600.f});

    view.setCenter({WindowWidth / 2.f, WindowHeight / 2.f});

    view.setRotation(sf::degrees(0.f));

    window.setView(view);

    sf::Vector2i windowCenter(window.getSize().x / 2, window.getSize().y / 2);
    sf::Mouse::setPosition(windowCenter, window);

    targetPos = {LevelWidth / 2.f, LevelHeight / 2.f};

    Clock clock;
    while (window.isOpen() && isLevelRunning)
    {
        deltaTime = clock.restart().asSeconds();
        if (deltaTime > 0.1f)
            deltaTime = 0.016f;
        totaltime += deltaTime;

        while (const optional event = window.pollEvent())
        {
            if (event->is<Event::Closed>())
                window.close();

            if (const auto *keyPressed = event->getIf<Event::KeyPressed>())
            {
                if (keyPressed->code == Keyboard::Key::Escape)
                {
                    ShowPauseMenu(); 
                }
            }
        }

        Updatemovingplayer(deltaTime);
        UpdateSmallFishes(deltaTime);
        UpdateMediumFishes(deltaTime);
        UpdateLargeFishes(deltaTime);
        UpdateGameBubbles(deltaTime);
        UpdateMermaidEvent(deltaTime);
        UpdatePowerUps(deltaTime);
        UpdateLevelHud();

        if (!isLevelRunning && goToMainMenuFromLevel)
        {
            QuitLevelLoadingScreen();
            break;
           
        }
     
        if (isGameOver && !isPlayerDead)
        {
            isLevelRunning = false;
            goToMainMenuFromLevel = true;
        }

        Vector2f playerPos = sprPlayerall.getPosition();
        Vector2f camCenter = view.getCenter();
        camCenter.x += (playerPos.x - camCenter.x) * 5.f * deltaTime;
        camCenter.y += (playerPos.y - camCenter.y) * 5.f * deltaTime;

        camCenter.x = max(WindowWidth / 2.f, min(camCenter.x, LevelWidth - WindowWidth / 2.f));
        camCenter.y = max(WindowHeight / 2.f, min(camCenter.y, LevelHeight - WindowHeight / 2.f));

        view.setCenter(camCenter);
        window.setView(view);

        window.clear();
        Drawbglevel();
        window.display();
    }
    window.setMouseCursorVisible(true);
}

void Timeattacklevel()
{
    currentGamemode = TIMEATTACK;
    gameScreenActive = false;
    bool isGameWon = false;
    float finalTime = 0.f;
    isLevelRunning = true;
    goToMainMenuFromLevel = false;
    Startbglevel();
    StartLevelHud();
    Startmovingplayer();

    sf::Text timerText(font);
    timerText.setCharacterSize(60);
    timerText.setFillColor(sf::Color::White);
    timerText.setPosition({300.f, 250.f});

    
    const sf::Time totalDuration = sf::seconds(90.f);
    sf::Clock countdownClock;
    view.setCenter({WindowWidth / 2.f, WindowHeight / 2.f});
    window.setView(view);

    Clock clock;
    while (window.isOpen() && isLevelRunning)
    {
        float deltaTime = clock.restart().asSeconds();
        if (deltaTime > 0.1f)
            deltaTime = 0.016f;
        totaltime += deltaTime;

        while (const optional event = window.pollEvent())
        {
            if (event->is<Event::Closed>())
                window.close();

            if (const auto *keyPressed = event->getIf<Event::KeyPressed>())
            {
                if (keyPressed->code == Keyboard::Key::Escape)
                {
                    ShowPauseMenu(); 
                }
            }
        }

        Updatemovingplayer(deltaTime);
        UpdateSmallFishes(deltaTime);
        UpdateMediumFishes(deltaTime);
        UpdateLargeFishes(deltaTime);
        UpdateGameBubbles(deltaTime);
        UpdateMermaidEvent(deltaTime);
        UpdatePowerUps(deltaTime);
        UpdateLevelHud();

        if (!isLevelRunning && goToMainMenuFromLevel)
        {
            QuitLevelLoadingScreen();
            break;
            
        }

       
        if (isGameOver && !isPlayerDead)
        {
            isLevelRunning = false;
            goToMainMenuFromLevel = true;
        }

        Vector2f playerPos = sprPlayerall.getPosition();
        Vector2f camCenter = view.getCenter();
        camCenter.x += (playerPos.x - camCenter.x) * 5.f * deltaTime;
        camCenter.y += (playerPos.y - camCenter.y) * 5.f * deltaTime;

        camCenter.x = max(WindowWidth / 2.f, min(camCenter.x, LevelWidth - WindowWidth / 2.f));
        camCenter.y = max(WindowHeight / 2.f, min(camCenter.y, LevelHeight - WindowHeight / 2.f));

        view.setCenter(camCenter);
        window.setView(view);

        window.clear();
        Drawbglevel();

        window.display();
    }
    window.setMouseCursorVisible(true);
}

void Startbglevel()
{

    if (!swayShader.loadFromFile("sway.frag", Shader::Type::Fragment))
    {
        cout << "Error loading sway shader!" << endl;
    }

    if (!swayShaderPlants.loadFromFile("sway_plants.frag", Shader::Type::Fragment))
    {
        cout << "Error loading sway_plants shader!" << endl;
    }
    if (!smallFishTexture.loadFromFile("Assets/Fish/herfish/herFish.png"))
    {
        cout << "Error loading small fish texture!" << endl;
    }

    if (!mediumFishTexture.loadFromFile("Assets/Fish/cod/cod.png"))
    {
        cout << "Error loading Medium Fish texture!" << endl;
    }

    if (!largeFishTexture.loadFromFile("Assets/Fish/lionfish/lionfish.png"))
    {
        cout << "Error loading Large Fish texture!" << endl;
    }

    largeFishTexture.setSmooth(true);
    mediumFishTexture.setSmooth(true);
    smallFishTexture.setSmooth(true);
    texbglevel.setSmooth(true);
    reefsgrass.setSmooth(true);
    reefsledge2.setSmooth(true);
    reefsledge.setSmooth(true);
    reefsoverhang.setSmooth(true);
    reefsplants.setSmooth(true);
    reefsplants2.setSmooth(true);
    reefsseaweed.setSmooth(true);
    reefsseaweed2.setSmooth(true);
    reefsseaweed3.setSmooth(true);
    reefsseaweed4.setSmooth(true);
    reefstubes.setSmooth(true);
    reefstubes2.setSmooth(true);
    reefstubes3.setSmooth(true);
    reefsplants23.setSmooth(true);
    reefsseaweed22.setSmooth(true);
    sprreefsgrass.setTexture(reefsgrass);

    Vector2u grassSize = reefsgrass.getSize();

    float grassScale = 0.4f;
    sprreefsgrass.setScale({grassScale, grassScale});

    float grassX = LevelWidth - 160.f;
    float grassY = LevelHeight - (grassSize.y * grassScale) + 10.f;

    sprreefsgrass.setPosition({grassX, grassY});

    sprreefsseaweed3.setTexture(reefsseaweed3);

    grassSize = reefsseaweed3.getSize();
    grassScale = 0.4f;
    sprreefsseaweed3.setScale({grassScale, grassScale});

    grassX = LevelWidth - 35.f;
    grassY = LevelHeight - (grassSize.y * grassScale) + 10.f;

    sprreefsseaweed3.setPosition({grassX, grassY});

    sprreefstubes3.setTexture(reefstubes3);

    grassSize = reefstubes3.getSize();
    grassScale = 1.3f;
    sprreefstubes3.setScale({grassScale, grassScale});

    grassX = LevelWidth - 135.f;
    grassY = LevelHeight - (grassSize.y * grassScale) + 10.f;

    sprreefstubes3.setPosition({grassX, grassY});

    sprreefstubes.setTexture(reefstubes);

    grassSize = reefstubes.getSize();
    grassScale = 0.65f;
    sprreefstubes.setScale({grassScale, grassScale});

    grassX = LevelWidth - 210.f;
    grassY = LevelHeight - (grassSize.y * grassScale) + 10.f;

    sprreefstubes.setPosition({grassX, grassY});

    sprreefsseaweed.setTexture(reefsseaweed);

    grassSize = reefsseaweed.getSize();
    grassScale = 0.45f;
    sprreefsseaweed.setScale({grassScale, grassScale});

    grassX = LevelWidth / 2.f;
    grassY = LevelHeight - (grassSize.y * grassScale) + 10.f;

    sprreefsseaweed.setPosition({grassX, grassY});

    sprreefsseaweed22.setTexture(reefsseaweed22);

    grassSize = reefsseaweed22.getSize();
    grassScale = 0.45f;
    sprreefsseaweed22.setScale({grassScale, grassScale});

    grassX = LevelWidth / 2.f - 80.f;
    grassY = LevelHeight - (grassSize.y * grassScale) + 10.f;

    sprreefsseaweed22.setPosition({grassX, grassY});

    sprreefsplants2.setTexture(reefsplants2);

    grassSize = reefsplants2.getSize();
    grassScale = 0.55f;
    sprreefsplants2.setScale({grassScale, grassScale});

    grassX = 10.f;
    grassY = LevelHeight - (grassSize.y * grassScale) + 10.f;

    sprreefsplants2.setPosition({grassX, grassY});

    sprreefsplants23.setTexture(reefsplants23);

    grassSize = reefsplants23.getSize();
    grassScale = 0.40f;
    sprreefsplants23.setScale({grassScale, grassScale});

    grassX = -10.f;
    grassY = LevelHeight - (grassSize.y * grassScale) + 10.f;

    sprreefsplants23.setPosition({grassX, grassY});

    sprreefsseaweed31.setTexture(reefsseaweed31);

    grassSize = reefsseaweed31.getSize();
    grassScale = 0.45f;
    sprreefsseaweed31.setScale({grassScale, grassScale});

    grassX = LevelWidth / 2.f - 255.f;
    grassY = LevelHeight - (grassSize.y * grassScale) + 130.f;

    sprreefsseaweed31.setPosition({grassX, grassY});

    sprreefsseaweed32.setTexture(reefsseaweed32);

    grassSize = reefsseaweed32.getSize();
    grassScale = 0.40f;
    sprreefsseaweed32.setScale({grassScale, grassScale});

    grassX = -10.f;
    grassY = LevelHeight - (grassSize.y * grassScale) + 10.f;

    sprreefsseaweed32.setPosition({grassX, grassY});

    sprreefsseaweed33.setTexture(reefsseaweed33);

    grassSize = reefsseaweed33.getSize();
    grassScale = 0.40f;
    sprreefsseaweed33.setScale({grassScale, grassScale});

    grassX = -10.f;
    grassY = LevelHeight - (grassSize.y * grassScale) + 10.f;

    sprreefsseaweed33.setPosition({grassX, grassY});

    for (int i = 0; i < 60; i++)
    {
        ventBubbles[i].active = false;
        ventBubbles[i].alpha = 0.f;
        ventBubbles[i].x = 0.f;
        ventBubbles[i].y = 0.f;
        ventBubbles[i].vx = 0.f;
        ventBubbles[i].vy = 0.f;
    }
}

void Drawbglevel()
{
    window.setMouseCursorVisible(false);
    window.setView(view);

    swayShader.setUniform("time", totaltime);
    window.draw(sprbglevel, &swayShader);

    swayShaderPlants.setUniform("time", totaltime);
    window.draw(sprreefstubes, &swayShaderPlants);

    DrawLargeFishes(window);
    DrawMediumFishes(window);
    DrawSmallFishes(window);
    Drawmovingplayer();
    DrawGameBubbles();
    DrawMermaidEvent();
    DrawPowerUps();

    for (int i = 0; i < MAX_POPUPS; i++)
    {
        if (scorePopups[i].active)
        {
            window.draw(scorePopups[i].text);
        }
    }

    window.draw(sprreefsplants2, &swayShaderPlants);
    window.draw(sprreefsseaweed31, &swayShaderPlants);
    window.draw(sprreefsseaweed22, &swayShaderPlants);
    window.draw(sprreefsseaweed, &swayShaderPlants);
    window.draw(sprreefsgrass, &swayShaderPlants);
    window.draw(sprreefsplants23, &swayShaderPlants);


    // ==========================================
    // --- Vent Bubbles Logic for Level ---
    // ==========================================
    
    static Vector2f levelVentPos1 = {LevelWidth - 133.f, LevelHeight - 130.f};
    static Vector2f levelVentPos2 = {LevelWidth - 198.f, LevelHeight - 110.f};
    
    static float levelVentTimer1 = 0.f;
    static float levelVentTimer2 = 0.f;

    levelVentTimer1 += deltaTime;
    levelVentTimer2 += deltaTime;

    auto spawnLevelBubble = [&](Vector2f ventPos) {
        for (int i = 0; i < 60; i++)
        {
            if (!ventBubbles[i].active)
            {
                ventBubbles[i].active = true;
                ventBubbles[i].isVent = true;
                ventBubbles[i].alpha = 255.0f;
                ventBubbles[i].x = ventPos.x + (rand() % 12 - 6);
                ventBubbles[i].y = ventPos.y;
                
                ventBubbles[i].vx = (float)(rand() % 10 - 5) / 150.0f;
                ventBubbles[i].vy = -((float)(rand() % 20 + 20)) / 50.0f;
                return true;
            }
        }
        return false;
    };

    if (levelVentTimer1 >= 0.8f)
    {
        if (spawnLevelBubble(levelVentPos1))
            levelVentTimer1 = 0.0f;
    }
    if (levelVentTimer2 >= 0.8f)
    {
        if (spawnLevelBubble(levelVentPos2))
            levelVentTimer2 = 0.0f;
    }

    for (int i = 0; i < 60; i++)
    {
        if (ventBubbles[i].active)
        {
            ventBubbles[i].x += ventBubbles[i].vx;
            ventBubbles[i].y += ventBubbles[i].vy;

            if (ventBubbles[i].isVent)
            {
                ventBubbles[i].x += sin(totaltime * 4.0f + i) * 0.25f;
                
                if (ventBubbles[i].y < 200.f)
                    ventBubbles[i].alpha -= 8.0f;

                if (ventBubbles[i].alpha <= 0 || ventBubbles[i].y < -100.f)
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
    // ==========================================
    // --- Vent Bubbles ---
    // ==========================================
    
    if (showSorryAnimation && !sorryExploded)
    {
        float centerX = view.getCenter().x;
        float centerY = view.getCenter().y - 100.f;

        std::string sorryStr = "SORRY";
        float spacing = 60.f;
        float startX = centerX - (2.f * spacing);

        for (int i = 0; i < 5; i++)
        {
            if (sorryLetterScales[i] > 0.01f)
            {
                float curX = startX + (i * spacing);
                float offsetY = sin(totaltime * 3.0f + i) * 5.f; 

               
                bubbleSprite.setScale({0.9f * sorryLetterScales[i], 0.9f * sorryLetterScales[i]});
                bubbleSprite.setOrigin(bubbleSprite.getLocalBounds().size / 2.f);
                bubbleSprite.setPosition({curX, centerY + offsetY});
                window.draw(bubbleSprite);

               
                static sf::Text letterText(font);
                letterText.setString(std::string(1, sorryStr[i]));
                letterText.setCharacterSize(35);
                letterText.setFillColor(sf::Color(255, 50, 50));
                letterText.setOutlineColor(sf::Color::Black);
                letterText.setOutlineThickness(2.f);

                
                sf::FloatRect b = letterText.getLocalBounds();
                letterText.setOrigin({b.position.x + b.size.x / 2.f, b.position.y + b.size.y / 2.f});
                letterText.setScale({sorryLetterScales[i], sorryLetterScales[i]});

                letterText.setPosition({curX, centerY + offsetY});

                window.draw(letterText);
            }
        }
    }

   
    if (showPerfectAnimation && !perfectExploded)
    {
        float centerX = view.getCenter().x;
        float centerY = view.getCenter().y - 100.f;

        std::string perfectStr = "PERFECT";
        float spacing = 60.f;
        float startX = centerX - (3.f * spacing);

        for (int i = 0; i < 7; i++)
        {
            if (perfectLetterScales[i] > 0.01f)
            {
                float curX = startX + (i * spacing);
                float offsetY = sin(totaltime * 3.0f + i) * 5.f;

        
                bubbleSprite.setScale({0.9f * perfectLetterScales[i], 0.9f * perfectLetterScales[i]});
                bubbleSprite.setOrigin(bubbleSprite.getLocalBounds().size / 2.f);
                bubbleSprite.setPosition({curX, centerY + offsetY});
                window.draw(bubbleSprite);

            
                static sf::Text letterText(font);
                letterText.setString(std::string(1, perfectStr[i]));
                letterText.setCharacterSize(35);

               
                letterText.setFillColor(sf::Color(160, 211, 74));
                letterText.setOutlineColor(sf::Color::Black);
                letterText.setOutlineThickness(2.f);

                sf::FloatRect b = letterText.getLocalBounds();
                letterText.setOrigin({b.position.x + b.size.x / 2.f, b.position.y + b.size.y / 2.f});
                letterText.setScale({perfectLetterScales[i], perfectLetterScales[i]});

                letterText.setPosition({curX, centerY + offsetY});

                window.draw(letterText);
            }
        }
    }

   
    for (int i = 0; i < 35; i++)
    {
        if (sparks[i].alpha <= 0)
        {
            sparks[i].active = false;
        }

        if (sparks[i].active)
        {
         
            sparks[i].x += sparks[i].vx;
            sparks[i].y += sparks[i].vy;

          

            sparks[i].vy += (float)(rand() % 10 - 5) / 500.0f;

            
            sparks[i].alpha -= 1.0f;

            sparkSprite.setPosition({sparks[i].x, sparks[i].y});
            sparkSprite.setColor(Color(255, 255, 255, (uint8_t)sparks[i].alpha));
            sparkSprite.setScale({0.6f, 0.6f});
            window.draw(sparkSprite);
        }
    }

    DrawLevelHud(false);
}

void createScorePopup(float x, float y, int points)
{
    for (int i = 0; i < MAX_POPUPS; i++)
    {
        if (!scorePopups[i].active)
        {
            scorePopups[i].active = true;
            scorePopups[i].alpha = 255.f;

            scorePopups[i].text.setFont(font);
            scorePopups[i].text.setString("+" + std::to_string(points));
            scorePopups[i].text.setCharacterSize(30);

            sf::Color textColor = sf::Color::White;
            switch (multiplier)
            {
            case 1: textColor = sf::Color::White; break;
            case 2: textColor = sf::Color(0, 255, 0); break;
            case 3: textColor = sf::Color(255, 165, 0); break;
            case 4: textColor = sf::Color(21, 145, 234); break;
            case 5: textColor = sf::Color::Yellow; break;
            case 6: textColor = sf::Color::Red; break;
            }

            scorePopups[i].text.setFillColor(textColor);
            scorePopups[i].baseColor = textColor;

            scorePopups[i].text.setOutlineThickness(2);
            scorePopups[i].text.setOutlineColor(sf::Color::Black);

            sf::FloatRect bounds = scorePopups[i].text.getLocalBounds();
            scorePopups[i].text.setOrigin({bounds.size.x / 2.f, bounds.size.y / 2.f});
            scorePopups[i].text.setPosition({x, y});
            break;
        }
    }
}

void createScorePopup(float x, float y, String power)
{
    for (int i = 0; i < MAX_POPUPS; i++)
    {
        if (!scorePopups[i].active)
        {
            scorePopups[i].active = true;
            scorePopups[i].alpha = 255.f;

            scorePopups[i].text.setFont(font);
            scorePopups[i].text.setString(power);
            scorePopups[i].text.setCharacterSize(30);

            sf::Color textColor = sf::Color::White;
            switch (multiplier)
            {
            case 1: textColor = sf::Color::White; break;
            case 2: textColor = sf::Color(0, 255, 0); break;
            case 3: textColor = sf::Color(255, 165, 0); break;
            case 4: textColor = sf::Color(21, 145, 234); break;
            case 5: textColor = sf::Color::Yellow; break;
            case 6: textColor = sf::Color::Red; break;
            }

            scorePopups[i].text.setFillColor(textColor);
            scorePopups[i].baseColor = textColor;

            scorePopups[i].text.setOutlineThickness(2);
            scorePopups[i].text.setOutlineColor(sf::Color::Black);

            sf::FloatRect bounds = scorePopups[i].text.getLocalBounds();
            scorePopups[i].text.setOrigin({bounds.size.x / 2.f, bounds.size.y / 2.f});
            scorePopups[i].text.setPosition({x, y});
            break;
        }
    }
}

void Movingplayer()
{
    Startmovingplayer();

    sf::Clock deltaClock;
    while (window.isOpen())
    {

        while (const std::optional<sf::Event> event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        float dt = deltaClock.restart().asSeconds();
        Updatemovingplayer(dt);

        window.clear(sf::Color(0, 105, 148));
        Drawmovingplayer();
        window.display();
    }
}

void Startmovingplayer()
{

    static float *pCurrentVisualScale = nullptr;
    static bool *pCanDash = nullptr;
    static bool *pIsDashingNow = nullptr;
    static bool *pWasMousePressed = nullptr;
    static float *pCurrentRotation = nullptr;
    

    isPlayerDead = false;
    isGameOver = false;
    isEscapeMode = false;
    stopSpawning = false;
    isMermaidEventActive = false;
    mermaidFinished = false;
    showPerfectAnimation = false;
    perfectExploded = false;
    perfectTimer = 0.f;
    for (int i = 0; i < 7; i++)
        perfectLetterScales[i] = 0.f;
    showSorryAnimation = false;
    sorryExploded = false;
    sorryTimer = 0.f;
    isGameWon = false;
    mermaidStarted = false;
    hasPlayedExitSound = false;
    anyFishLeft = false;
    lives = 3;
    score = (currentGamemode == CLASSIC) ? storyCarryScore : taCarryScore;
    starsEatenCount = 0; 
    largeFishEatenCount = 0;
    mediumFishEatenCount = 0;
    smallFishEatenCount = 0;
    multiplier = 1;
    comboProgress = 0.f;
    comboState = FILLING;
    playerLevel = 1;
    fishEatenCount = 0; 

    for (int i = 0; i < MAX_SMALL_FISH; i++)
    {
        if (smallFishes[i].sprite)
        {
            delete smallFishes[i].sprite;
            smallFishes[i].sprite = nullptr;
        }
        smallFishes[i].active = false;
        smallFishes[i].isFleeing = false;
        smallFishes[i].hasTurned = false;
        smallFishes[i].isTurning = false;
    }
    for (int i = 0; i < MAX_MEDIUM_FISH; i++)
    {
        if (mediumFishes[i].sprite)
        {
            delete mediumFishes[i].sprite;
            mediumFishes[i].sprite = nullptr;
        }
        mediumFishes[i].active = false;
        mediumFishes[i].isFleeing = false;
        mediumFishes[i].hasTurned = false;
        mediumFishes[i].state = 1;
    }
    for (int i = 0; i < MAX_LARGE_FISH; i++)
    {
        if (largeFishes[i].sprite)
        {
            delete largeFishes[i].sprite;
            largeFishes[i].sprite = nullptr;
        }
        largeFishes[i].active = false;
        largeFishes[i].isFleeing = false;
        largeFishes[i].hasTurned = false;
        largeFishes[i].state = 1;
    }


    for (int i = 0; i < 35; i++)
    {
        sparks[i].active = false;
        sparks[i].alpha = 0.f;
    }
    gameBubbleSpawnTimer = 0.f;

    
    (void)texPlayerall.loadFromFile("Assets/Fish/butterflyfish/Butterflyfishall.png");
    spawnDelayClock.restart();
    sprPlayerall.setTexture(texPlayerall);

    sprPlayerall.setOrigin({DRAW_W / 2.0f, DRAW_H / 2.0f});


    float startScale = FISH_SCALE;
    if (playerLevel == 2)
        startScale = 0.4f;
    else if (playerLevel == 3)
        startScale = 0.6f;

    currentVisualScale = startScale; 

    isFacingRight = true;
    sprPlayerall.setScale({-currentVisualScale, currentVisualScale}); 

    playerIntroActive = true;
    playerIntroStep = 0;
    hasPlayedSpawnSound = false; 

    sprPlayerall.setPosition({LevelWidth / 2.0f, LevelHeight / 2.0f});
    
    texPlayerall.setSmooth(true);
    levelsound.setLooping(true);
    levelsound.play();
    WaveSound.play();
    levelsound.setLooping(true);
    WaveSound.setLooping(true);
    
    StartPowerUps();

    currentState = IDLE;
    currentFrame = 0;
    isFacingRight = false;
    lastMouseX = window.mapPixelToCoords(sf::Mouse::getPosition(window), view).x;

    fishEatenCount = 0; 
    playerLevel = 1;    
    if (currentGamemode == TIMEATTACK)
    {
        remainingTime = timeAttackDuration; 
        timerText.setFont(font);
        timerText.setCharacterSize(40);
        timerText.setFillColor(sf::Color::White);
        timerText.setOutlineThickness(2);
        timerText.setOutlineColor(sf::Color::Black);
    }
    else
    {
        remainingTime = 0;
    } 

    targetPos = sprPlayerall.getPosition(); 
    sf::Vector2i windowCenter(window.getSize().x / 2, window.getSize().y / 2);
    sf::Mouse::setPosition(windowCenter, window);
}

void Updatemovingplayer(float dt)
{
    timer += dt;

    if (currentGamemode == TIMEATTACK && !isPlayerDead)
    {
  
        if (!isGameWon && remainingTime > 0)
        {
            remainingTime -= dt;


            if (remainingTime < 0.f)
                remainingTime = 0.f;

            std::stringstream ss;
            int minutes = static_cast<int>(remainingTime) / 60;
            int seconds = static_cast<int>(remainingTime) % 60;
            ss << std::setfill('0') << std::setw(2) << minutes << ":"
               << std::setfill('0') << std::setw(2) << seconds;
            timerText.setString("Time: " + ss.str());

            if (remainingTime <= 10)
            {
                timerText.setFillColor(sf::Color::Red);
            }
        }

        if (remainingTime <= 0.f && !isGameOver)
        {
            isGameOver = true;
            lives = 0;
            isPlayerDead = true;
            isEscapeMode = true;
            stopSpawning = true;
            gameover.play();

            showSorryAnimation = true;
            sorryExploded = false;
            sorryTimer = 0.f;
            for (int i = 0; i < 5; i++)
            {
                sorryLetterScales[i] = 0.f;
            }
            respawnClock.restart();
        }


        if (!isGameWon && fishEatenCount >= currentLevelSettings.fishToWin)
        {
            isGameWon = true;
            finalTime = timeAttackDuration - remainingTime;
            stopSpawning = true; 
			isEscapeMode = true; 
            if (!mermaidStarted && !anyFishLeft)
            {
                StartMermaidEvent();
                showPerfectAnimation = true;
                mermaidStarted = true; 
            }
        }
    }
    // ==========================================

    // static float currentVisualScale = FISH_SCALE;

    if (isInvincible && hitCooldownClock.getElapsedTime().asSeconds() > 5.0f)
    {
        isInvincible = false;
    }

    // --- Combo Logic ---
    static bool hasEatenThisFrame = false;
    hasEatenThisFrame = false;
    switch (comboState)
    {
    case FILLING:
        comboTimer += dt;
        if (comboTimer >= WAIT_TIME)
        {
            comboState = WAIT_DRAIN;
            comboTimer = 0.0f;
        }
        break;
    case WAIT_DRAIN:
        comboTimer += dt;
        if (comboTimer >= WAIT_TIME)
        {
            comboState = DRAINING;
            comboTimer = 0.0f;
        }
        break;
    case DRAINING:
        comboProgress -= DRAIN_SPEED * dt;
        if (comboProgress <= 0.0f)
        {
            if (multiplier > 1)
            {
                multiplier--;          // Drop a tier
                comboProgress = 0.99f; // Instantly wrap around to drain the lower tier letter-by-letter!
            }
            else
            {
                comboProgress = 0.0f;
                comboState = FILLING; // Reached absolute zero
            }
        }
        break;
    case WAIT_DROP:
        break;
    }

    if (isPlayerDead)
    {
        float elapsed = respawnClock.getElapsedTime().asSeconds();

        if (lives <= 0 && dieSound.getStatus() == sf::Sound::Status::Stopped)
        {
            if (gameover.getStatus() != sf::Sound::Status::Playing)
            {
                gameover.play(); 
            }
        }

        if (showSorryAnimation)
        {
            sorryTimer += dt;

            float delay = 0.25f;

            for (int i = 0; i < 5; i++)
            {
                float t = sorryTimer - (i * delay);
                if (t > 0)
                {
                    sorryLetterScales[i] += dt * 1.5f;
                    if (sorryLetterScales[i] > 1.0f)
                        sorryLetterScales[i] = 1.0f;
                }
            }
        }

        if (elapsed >= 4.0f && !sorryExploded)
        {
            sorryExploded = true;
            Bubbledone.play();

            for (int i = 0; i < 35; i++)
            {
                int idx = i / 7;
                float pX = view.getCenter().x - (3 * 50.f) + (idx * 50.f);
                float pY = view.getCenter().y - 120.f;

                sparks[i].active = true;
                sparks[i].x = pX;
                sparks[i].y = pY;

                sparks[i].vx = (float)(rand() % 20 - 5) / 10.0f;
                sparks[i].vy = -((float)(rand() % 20 - 10)) / 10.0f;

                sparks[i].alpha = 255.0f;
            }
        }

        if (lives <= 0 && elapsed >= 5.0f)
        {
            addNewHighScore(userName, score, currentGamemode == CLASSIC);
            isLevelRunning = false;
            goToMainMenuFromLevel = true;
            return;
        }

        if (elapsed >= 5.0f)
        {
            isPlayerDead = false;
            isInvincible = true;
            hitCooldownClock.restart();
            playerIntroActive = true;
            playerIntroStep = 0;
            hasPlayedSpawnSound = false;
            spawnDelayClock.restart();
            levelsound.play();

            showSorryAnimation = false;
            sorryExploded = false;
            sorryTimer = 0.f;

            multiplier = 1;
            comboProgress = 0.0f;
            comboState = FILLING;

            pendingEat = false;
            currentVisualScale = FISH_SCALE;

            float targetScale = FISH_SCALE;
            if (playerLevel == 2)
                targetScale = 0.4f;
            else if (playerLevel == 3)
                targetScale = 0.6f;

            isFacingRight = true;
            sprPlayerall.setScale({-currentVisualScale, currentVisualScale});
            sprPlayerall.setColor(Color::White);

            if (playerLevel == 1)
            {
                fishEatenCount = 0;
            }
            else if (playerLevel == 2)
            {
                fishEatenCount = currentLevelSettings.fishToGrowToLevel2;
            }
            else if (playerLevel == 3)
            {
                fishEatenCount = currentLevelSettings.fishToGrowToLevel3;
            }
        }

        for (int i = 0; i < MAX_POPUPS; i++)
        {
            if (scorePopups[i].active)
            {
                sf::Vector2f pos = scorePopups[i].text.getPosition();
                pos.y += scorePopups[i].ySpeed * dt;
                scorePopups[i].text.setPosition(pos);
                scorePopups[i].alpha -= 150.f * dt;
                if (scorePopups[i].alpha <= 0.f)
                    scorePopups[i].active = false;
                else
                {
                    scorePopups[i].text.setFillColor(sf::Color(scorePopups[i].baseColor.r, scorePopups[i].baseColor.g, scorePopups[i].baseColor.b, (uint8_t)scorePopups[i].alpha));
                    scorePopups[i].text.setOutlineColor(sf::Color(0, 0, 0, (uint8_t)scorePopups[i].alpha));
                }
            }
        }
        return;
    }

    // --- Intro Logic ---
    float introTargetY = LevelHeight / 2.0f;
    float sinkDepth = 40.0f;
    float sinkTargetY = introTargetY + sinkDepth;

    if (playerIntroActive)
    {
        sf::Vector2f pos = sprPlayerall.getPosition();
        if (playerIntroStep == 0)
        {
            pos.x = LevelWidth / 2.0f;
            pos.y = -350.0f;
            sprPlayerall.setPosition(pos);
            isFacingRight = true;
            sprPlayerall.setScale({-currentVisualScale, currentVisualScale});
            sprPlayerall.setRotation(degrees(45.f));
            view.setCenter({pos.x, pos.y});
            playerIntroStep = 1;
        }
        else if (playerIntroStep == 1)
        {
            float fallSpeed = 700.0f;
            pos.x = LevelWidth / 2.0f;
            if (pos.y < introTargetY)
            {
                pos.y += fallSpeed * dt;
                sprPlayerall.setPosition(pos);
                view.setCenter({pos.x, pos.y});
                currentState = SWIM;
                if (timer >= 0.15f)
                {
                    timer = 0.0f;
                    currentFrame++;
                    if (currentFrame >= frameCounts[(int)SWIM])
                        currentFrame = 0;
                    int gridX = currentFrame * GRID_W;
                    int gridY = (int)SWIM * GRID_H;
                    int offsetX = (GRID_W - DRAW_W) / 2;
                    int offsetY = (GRID_H - DRAW_H) / 2;
                    sprPlayerall.setTextureRect(sf::IntRect({gridX + offsetX, gridY + offsetY}, {DRAW_W, DRAW_H}));
                }
            }
            else
            {
                pos.y = introTargetY;
                sprPlayerall.setPosition(pos);
                playerIntroStep = 2;
            }
        }
        else if (playerIntroStep == 2)
        {
            float sinkSpeed = 300.0f;
            pos.x = LevelWidth / 2.0f;
            if (pos.y < sinkTargetY)
            {
                pos.y += sinkSpeed * dt;
                sprPlayerall.setPosition(pos);
                currentState = SWIM;
                if (timer >= 0.15f)
                {
                    timer = 0.0f;
                    currentFrame++;
                    if (currentFrame >= frameCounts[(int)SWIM])
                        currentFrame = 0;
                    int gridX = currentFrame * GRID_W;
                    int gridY = (int)SWIM * GRID_H;
                    int offsetX = (GRID_W - DRAW_W) / 2;
                    int offsetY = (GRID_H - DRAW_H) / 2;
                    sprPlayerall.setTextureRect(sf::IntRect({gridX + offsetX, gridY + offsetY}, {DRAW_W, DRAW_H}));
                }
            }
            else
            {
                sprPlayerall.setRotation(degrees(0.f));
                playerIntroStep = 3;
            }
        }
        else if (playerIntroStep == 3)
        {
            float floatSpeed = 200.0f;
            pos.x = LevelWidth / 2.0f;
            if (pos.y > introTargetY)
            {
                pos.y -= floatSpeed * dt;
                sprPlayerall.setPosition(pos);
                currentState = SWIM;
                if (timer >= 0.15f)
                {
                    timer = 0.0f;
                    currentFrame++;
                    if (currentFrame >= frameCounts[(int)SWIM])
                        currentFrame = 0;
                    int gridX = currentFrame * GRID_W;
                    int gridY = (int)SWIM * GRID_H;
                    int offsetX = (GRID_W - DRAW_W) / 2;
                    int offsetY = (GRID_H - DRAW_H) / 2;
                    sprPlayerall.setTextureRect(sf::IntRect({gridX + offsetX, gridY + offsetY}, {DRAW_W, DRAW_H}));
                }
            }
            else
            {
                pos.y = introTargetY;
                sprPlayerall.setPosition(pos);
                playerIntroActive = false;
                currentState = IDLE;
                targetPos = pos;
                sf::Vector2i windowCenter(window.getSize().x / 2, window.getSize().y / 2);
                sf::Mouse::setPosition(windowCenter, window);
            }
        }
        return;
    }

    // --- Growth System ---
    float targetScale = FISH_SCALE;
    if (playerLevel == 2)
        targetScale = 0.4f;
    else if (playerLevel == 3)
        targetScale = 0.6f;

    if (std::abs(currentVisualScale - targetScale) > 0.01f)
    {
        currentVisualScale += (targetScale - currentVisualScale) * 3.0f * dt;
        if (isFacingRight)
            sprPlayerall.setScale({-currentVisualScale, currentVisualScale});
        else
            sprPlayerall.setScale({currentVisualScale, currentVisualScale});
    }

    // --- Movement Logic ---
    sf::Vector2i windowCenter(window.getSize().x / 2, window.getSize().y / 2);
    sf::Vector2i currentScreenMousePos = sf::Mouse::getPosition(window);
    sf::Vector2i screenDelta = currentScreenMousePos - windowCenter;
    float worldPerPixel = view.getSize().x / (float)window.getSize().x;
    targetPos.x += screenDelta.x * worldPerPixel;
    targetPos.y += screenDelta.y * worldPerPixel;
    sf::Mouse::setPosition(windowCenter, window);

    float margin = 30.0f;


    float hudCeiling = view.getCenter().y - 300.f + 90.f;

    if (targetPos.x < margin)
        targetPos.x = margin;
    if (targetPos.x > LevelWidth - margin)
        targetPos.x = LevelWidth - margin;
    if (targetPos.y < hudCeiling + margin)
        targetPos.y = hudCeiling + margin;
    if (targetPos.y > LevelHeight - margin)
        targetPos.y = LevelHeight - margin;

    Vector2f currentPos = sprPlayerall.getPosition();
    Vector2f toMouse = targetPos - currentPos;
    float distance = std::sqrt(toMouse.x * toMouse.x + toMouse.y * toMouse.y);

    Vector2f moveDirection(0.f, 0.f);
    bool isMouseMoving = false;
    if (distance > 4.0f)
    {
        moveDirection = toMouse / distance;
        isMouseMoving = true;
    }
    else
    {
        moveDirection = {(isFacingRight ? 1.0f : -1.0f), 0.0f};
    }

    // --- Dash Logic ---
    static bool canDash = true;
    static bool isDashingNow = false;
    static Clock dashTimer;
    static bool wasMousePressed = false;
    static sf::Vector2f dashDirection = {1.f, 0.f};

    bool isMousePressedNow = sf::Mouse::isButtonPressed(sf::Mouse::Button::Left);
    if (isMousePressedNow && !wasMousePressed && canDash && !isPlayerDead)
    {
        sf::Vector2f toTarget = targetPos - currentPos;
        float len = std::sqrt(toTarget.x * toTarget.x + toTarget.y * toTarget.y);
        if (len > 5.f)
            dashDirection = toTarget / len;
        else
            dashDirection = {(isFacingRight ? 1.0f : -1.0f), 0.0f};
        isDashingNow = true;
        canDash = false;
        dashTimer.restart();
        dashSound.play();
    }
    wasMousePressed = isMousePressedNow;
    if (isDashingNow)
    {
        if (dashTimer.getElapsedTime().asSeconds() > dashDuration)
            isDashingNow = false;
    }
    if (!canDash)
    {
        if (dashTimer.getElapsedTime().asSeconds() > dashCooldown)
            canDash = true;
    }

    bool isMoving = false;
    // ==========================================
    // --- Mouse Speed Options ---
    // ==========================================
    float baseSpeed = 1000.0f; 

    if (MSpeedIndex == 1)
    {
        baseSpeed = 750.0f;   // Low
    }
    else if (MSpeedIndex == 2)
    {
        baseSpeed = 1000.0f;  // Medium
    }
    else if (MSpeedIndex == 3)
    {
        baseSpeed = 1250.0f;  // Medium 2
    }
    else if (MSpeedIndex == 4)
    {
        baseSpeed = 1500.0f;  // High
    }

    // --- Escape Mode Speed ---
    if (isEscapeMode)
    {
        baseSpeed = baseSpeed * 2.5f;
    }

    float playerSpeed = speedBoostActive ? baseSpeed * SPEED_BOOST_MULT : baseSpeed;

    // dash system
    if (isDashingNow)
    {
        playerSpeed *= 1.5f;
        currentPos += dashDirection * playerSpeed * dt;
        currentPos.x = std::max(margin, std::min(currentPos.x, LevelWidth - margin));
        currentPos.y = std::max(hudCeiling + margin, std::min(currentPos.y, LevelHeight - margin)); // Stops dashing under HUD
        sprPlayerall.setPosition(currentPos);
        isMoving = true;
        if (dashDirection.x > 0.1f && !isFacingRight)
        {
            isFacingRight = true;
            sprPlayerall.setScale({-currentVisualScale, currentVisualScale});
        }
        else if (dashDirection.x < -0.1f && isFacingRight)
        {
            isFacingRight = false;
            sprPlayerall.setScale({currentVisualScale, currentVisualScale});
        }
        targetPos = currentPos;
        static float dashBubbleTimer = 0.f;
        dashBubbleTimer += dt;
        if (dashBubbleTimer > 0.02f)
        {
            dashBubbleTimer = 0.f;
            StartGameBubble(currentPos.x + getRandom(-20.f, 20.f), currentPos.y + getRandom(-10.f, 10.f), true);
        }
    }
    else if (isMouseMoving)
    {
        currentPos += moveDirection * playerSpeed * dt;
        sprPlayerall.setPosition(currentPos);
        isMoving = true;
    }

    // --- Rotation ---
    static float currentRotation = 0.f;
    float maxTiltAngle = 35.0f;
    float targetRotation = 0.f;
    if (currentState != TURN && currentState != EAT)
    {
        if (isDashingNow)
            targetRotation = dashDirection.y * maxTiltAngle;
        else
            targetRotation = moveDirection.y * maxTiltAngle;
        if (!isFacingRight)
            targetRotation *= -1.0f;
    }
    currentRotation += (targetRotation - currentRotation) * 10.0f * dt;
    sprPlayerall.setRotation(degrees(currentRotation));

    if (currentState != EAT && !pendingEat)
    {
        if (isMoving)
        {
            if (isDashingNow)
            {
                if (currentState != SWIM)
                    currentState = SWIM;
            }
            else
            {
                bool movingRight = (moveDirection.x > 0);
                if (movingRight != isFacingRight)
                {
                    if (currentState != TURN)
                    {
                        currentState = TURN;
                        currentFrame = 0;
                        timer = 0.f;
                    }
                }
                else
                {
                    if (currentState != TURN)
                        currentState = SWIM;
                }
            }
        }
        else
        {
            if (currentState != TURN)
                currentState = IDLE;
        }
    }

    // --- 5. Collisions (Player Eats Fish ONLY) ---
    sf::Vector2f playerPos = sprPlayerall.getPosition();
    sf::Vector2f mouthPos = playerPos;

    float scaleRatio = currentVisualScale / FISH_SCALE;
    float baseMouthOffset = 60.f;
    float baseEatRadius = 40.f;
    float dynamicMouthOffset = baseMouthOffset * scaleRatio;
    float dynamicEatRadius = baseEatRadius * scaleRatio;
    if (isDashingNow)
        dynamicEatRadius *= 1.5f;

    sf::Vector2f collisionDir = isDashingNow ? dashDirection : moveDirection;
    if (distance > 5.0f || isDashingNow)
        mouthPos += collisionDir * dynamicMouthOffset;
    else
    {
        if (isFacingRight)
            mouthPos.x += dynamicMouthOffset;
        else
            mouthPos.x -= dynamicMouthOffset;
    }

    // Small Fish
    for (int i = 0; i < MAX_SMALL_FISH; i++)
    {
        if (smallFishes[i].active && smallFishes[i].sprite && !smallFishes[i].isFleeing)
        {
            float dx = smallFishes[i].sprite->getPosition().x - mouthPos.x;
            float dy = smallFishes[i].sprite->getPosition().y - mouthPos.y;
            float dist = std::sqrt(dx * dx + dy * dy);
            if (dist < dynamicEatRadius)
            {
                eatSound.play();
                smallFishes[i].active = false;
                delete smallFishes[i].sprite;
                smallFishes[i].sprite = nullptr;
                int points = 10 * multiplier;
                score += points;
                createScorePopup(mouthPos.x, mouthPos.y, points);
                hasEatenThisFrame = true;
                comboState = FILLING;
                comboTimer = 0.0f;

                comboProgress += 0.145f;
                if (comboProgress >= 1.0f)
                {
                    if (multiplier < MAX_MULTIPLIER)
                    {
                        multiplier++;
                        if (multiplier == 2)
                        {
                            sounds[0]->play();
                        }
                        else if (multiplier == 3)
                        {
                            sounds[1]->play();
                        }
                        else if (multiplier == 4)
                        {
                            sounds[2]->play();
                        }
                        else if (multiplier == 5)
                        {
                            sounds[3]->play();
                        }
                        else if (multiplier == 6)
                        {
                            sounds[4]->play();
                        }
                        comboProgress -= 1.0f;
                    }
                    else
                    {
                        comboProgress = 1.0f;
                    }
                }

                fishEatenCount++;
                smallFishEatenCount++;
                for (int k = 0; k < 5; k++)
                    StartGameBubble(mouthPos.x + getRandom(-10.f, 10.f), mouthPos.y + getRandom(-10.f, 10.f), true);

                if (currentState == TURN)
                    pendingEat = true;
                else
                {
                    currentState = EAT;
                    currentFrame = 0;
                    timer = 0.f;
                }
            }
        }
    }

    // Medium Fish
    for (int i = 0; i < MAX_MEDIUM_FISH; i++)
    {
        if (mediumFishes[i].active && mediumFishes[i].sprite && !mediumFishes[i].isFleeing)
        {
            float dx = mediumFishes[i].sprite->getPosition().x - mouthPos.x;
            float dy = mediumFishes[i].sprite->getPosition().y - mouthPos.y;
            float dist = std::sqrt(dx * dx + dy * dy);

            if (dist < dynamicEatRadius + 10.f)
            {
                bool mediumShrunk = abs(mediumFishes[i].sprite->getScale().y) <= 0.21f;
                if (playerLevel >= 2 || mediumShrunk)
                {
                    eatSound2.play(); 
                    mediumFishes[i].active = false;
                    delete mediumFishes[i].sprite;
                    mediumFishes[i].sprite = nullptr;
                    int points = 20 * multiplier;
                    score += points;
                    createScorePopup(mouthPos.x, mouthPos.y, points);
                    hasEatenThisFrame = true;
                    comboState = FILLING;
                    comboTimer = 0.0f;

                    comboProgress += 0.145f;
                    if (comboProgress >= 1.0f)
                    {
                        if (multiplier < MAX_MULTIPLIER)
                        {
                            multiplier++;
                            comboProgress -= 1.0f;
                        }
                        else
                        {
                            comboProgress = 1.0f; 
                        }
                    }

                    fishEatenCount += 3;
                    mediumFishEatenCount++;
                    for (int k = 0; k < 5; k++)
                        StartGameBubble(mouthPos.x + getRandom(-10.f, 10.f), mouthPos.y + getRandom(-10.f, 10.f), true);

                    if (currentState == TURN)
                        pendingEat = true;
                    else
                    {
                        currentState = EAT;
                        currentFrame = 0;
                        timer = 0.f;
                    }
                }
            }
        }
    }

    // Large Fish
    for (int i = 0; i < MAX_LARGE_FISH; i++)
    {
        if (largeFishes[i].active && largeFishes[i].sprite && !largeFishes[i].isFleeing)
        {
            float dx = largeFishes[i].sprite->getPosition().x - mouthPos.x;
            float dy = largeFishes[i].sprite->getPosition().y - mouthPos.y;
            float dist = std::sqrt(dx * dx + dy * dy);

            if (dist < dynamicEatRadius + 20.f)
            {
                bool largeShrunk = abs(largeFishes[i].sprite->getScale().y) <= 0.21f;
                if (playerLevel >= 3 || largeShrunk)
                {
                    PlayergotEaten.play();
                    largeFishes[i].active = false;
                    delete largeFishes[i].sprite;
                    largeFishes[i].sprite = nullptr;
                    int points = 35 * multiplier;
                    score += points;
                    createScorePopup(mouthPos.x, mouthPos.y, points);
                    hasEatenThisFrame = true;
                    comboState = FILLING;
                    comboTimer = 0.0f;

                    comboProgress += 0.145f;
                    if (comboProgress >= 1.0f)
                    {
                        if (multiplier < MAX_MULTIPLIER)
                        {
                            multiplier++;
                            comboProgress -= 1.0f;
                        }
                        else
                        {
                            comboProgress = 1.0f; 
                        }
                    }

                    fishEatenCount += 5; // Count the 5 points!
                    largeFishEatenCount++;


                    for (int k = 0; k < 5; k++)
                        StartGameBubble(mouthPos.x + getRandom(-10.f, 10.f), mouthPos.y + getRandom(-10.f, 10.f), true);

                    if (currentState == TURN)
                        pendingEat = true;
                    else
                    {
                        currentState = EAT;
                        currentFrame = 0;
                        timer = 0.f;
                    }
                }
            }
        }
    }

    // --- Level Up & Animation Loop ---
    if (playerLevel == 1 && fishEatenCount >= currentLevelSettings.fishToGrowToLevel2)
    {
        playerLevel = 2;
        levelUpSound.play();
    }
    else if (playerLevel == 2 && fishEatenCount >= currentLevelSettings.fishToGrowToLevel3)
    {
        playerLevel = 3;
        levelUpSound.play();
    }

    if (fishEatenCount >= currentLevelSettings.fishToWin && !isEscapeMode)
    {
        isEscapeMode = true;
        stopSpawning = true;
    }
    float currentSpeed = 0.15f;
    if (currentState == TURN)
        currentSpeed = 0.04f;
    else if (currentState == EAT)
        currentSpeed = 0.08f;

    if (timer >= currentSpeed)
    {
        timer = 0.0f;
        currentFrame++;
        if (currentFrame >= frameCounts[(int)currentState])
        {
            currentFrame = 0;
            if (currentState == EAT)
            {
                currentState = IDLE;
                pendingEat = false;
            }
            else if (currentState == TURN)
            {
                isFacingRight = !isFacingRight;
                if (isFacingRight)
                    sprPlayerall.setScale({-currentVisualScale, currentVisualScale});
                else
                    sprPlayerall.setScale({currentVisualScale, currentVisualScale});

                if (pendingEat)
                {
                    currentState = EAT;
                    pendingEat = false;
                }
                else
                {
                    if (isMoving)
                        currentState = SWIM;
                    else
                        currentState = IDLE;
                }
            }
        }
        int gridX = currentFrame * GRID_W;
        int gridY = (int)currentState * GRID_H;
        int offsetX = (GRID_W - DRAW_W) / 2;
        int offsetY = (GRID_H - DRAW_H) / 2;
        sprPlayerall.setTextureRect(sf::IntRect({gridX + offsetX, gridY + offsetY}, {DRAW_W, DRAW_H}));
    }

    // Score Popups Update
    for (int i = 0; i < MAX_POPUPS; i++)
    {
        if (scorePopups[i].active)
        {
            sf::Vector2f pos = scorePopups[i].text.getPosition();
            pos.y += scorePopups[i].ySpeed * dt;
            scorePopups[i].text.setPosition(pos);
            scorePopups[i].alpha -= 150.f * dt;
            if (scorePopups[i].alpha <= 0.f)
                scorePopups[i].active = false;
            else
            {
                scorePopups[i].text.setFillColor(sf::Color(scorePopups[i].baseColor.r, scorePopups[i].baseColor.g, scorePopups[i].baseColor.b, (uint8_t)scorePopups[i].alpha));
                scorePopups[i].text.setOutlineColor(sf::Color(0, 0, 0, (uint8_t)scorePopups[i].alpha));
            }
        }
    }

    if (isEscapeMode && !isMermaidEventActive)
    {
         anyFishLeft = false;
        for (int i = 0; i < MAX_SMALL_FISH; i++)
            if (smallFishes[i].active)
                anyFishLeft = true;
        for (int i = 0; i < MAX_MEDIUM_FISH; i++)
            if (mediumFishes[i].active)
                anyFishLeft = true;
        for (int i = 0; i < MAX_LARGE_FISH; i++)
            if (largeFishes[i].active)
                anyFishLeft = true;

        if (!anyFishLeft)
        {
            StartMermaidEvent();
        }
    }


    if (showPerfectAnimation)
    {
        perfectTimer += dt;

        if (!perfectExploded)
        {
            float delay = 0.25f;
            for (int i = 0; i < 7; i++)
            {
                float t = perfectTimer - (i * delay);
                if (t > 0)
                {
                    perfectLetterScales[i] += dt * 1.5f;
                    if (perfectLetterScales[i] > 1.0f)
                        perfectLetterScales[i] = 1.0f;
                }
            }
        }


        if (perfectTimer >= 4.0f && !perfectExploded)
        {
            perfectExploded = true;
            Bubbledone.play();

            for (int i = 0; i < 35; i++)
                sparks[i].active = false;

            for (int i = 0; i < 35; i++)
            {
                int idx = i / 5;


                float centerX = view.getCenter().x;
                float centerY = view.getCenter().y - 100.f;
                float spacing = 60.f;
                float startX = centerX - (3.f * spacing); 

                float pX = startX + (idx * spacing);
                float pY = centerY;

                sparks[i].active = true;
                sparks[i].x = pX;
                sparks[i].y = pY;


                sparks[i].vx = (float)(rand() % 20 - 5) / 10.0f;
                sparks[i].vy = -((float)(rand() % 20 - 10)) / 10.0f;

                sparks[i].alpha = 255.0f;
            }
        }

        if (perfectTimer >= 6.0f)
        {
            EndLevel();
        }
    }
}

void Drawmovingplayer()
{
    if (!isPlayerDead)
    {

        if (isInvincible)
        {

            float fadeValue = (std::sin(totaltime * 10.0f) + 1.0f) * 0.75f; 
            int alpha = static_cast<int>(fadeValue * 255);                 

            sprPlayerall.setColor(sf::Color(255, 255, 255, alpha));
        }
        else
        {
            sprPlayerall.setColor(sf::Color::White);
        }

        window.draw(sprPlayerall);


        sprPlayerall.setColor(sf::Color::White);
    }
}

void StartSmallFish()
{
    int freeIndex = -1;
    for (int i = 0; i < MAX_SMALL_FISH; i++)
    {
        if (smallFishes[i].active == false)
        {
            freeIndex = i;
            break;
        }
    }
    if (freeIndex == -1)
        return;

    SmallFish &newFish = smallFishes[freeIndex];
    newFish.sprite = new sf::Sprite(minowfishtex);

    float fishScale = 0.2f;
    newFish.sprite->setOrigin(sf::Vector2f(286 / 2.f, 126 / 2.f));

    newFish.active = true;
    newFish.isFleeing = false;
    newFish.isTurning = false;
    newFish.currentFrame = 0;

    newFish.canTurn = (rand() % 4 == 0);
    newFish.hasTurned = false;

    if (newFish.canTurn)
    {
        newFish.timeToNextTurn = 3.0f + static_cast<float>(rand() % 5);
    }

    float randomSpeed = 40.0f + static_cast<float>(rand() % 40);

    float randomYSpeed = static_cast<float>(rand() % 60 - 30);

    float hudCeiling = view.getCenter().y - 300.f + 90.f;
    float minY = hudCeiling + 20.f; // Extra 20 for their body size
    float maxY = LevelHeight - 50.0f;
    if (maxY <= minY)
        maxY = minY + 10.f;
    float randomY = minY + (rand() % (int)(maxY - minY));

    int side = rand() % 2;

    sf::Vector2f viewCenter = view.getCenter();
    sf::Vector2f viewSize = view.getSize();
    float viewLeft = viewCenter.x - viewSize.x / 2.0f;
    float viewRight = viewCenter.x + viewSize.x / 2.0f;

    if (side == 0)
    {
        newFish.sprite->setPosition(sf::Vector2f(viewLeft - 200.f, randomY));
        newFish.velocity = {randomSpeed, randomYSpeed};
        newFish.originalSpeedX = randomSpeed;
        newFish.sprite->setScale(sf::Vector2f(-fishScale, fishScale));
    }
    else
    {
        newFish.sprite->setPosition(sf::Vector2f(viewRight + 200.f, randomY));
        newFish.velocity = {-randomSpeed, randomYSpeed};
        newFish.originalSpeedX = -randomSpeed;
        newFish.sprite->setScale(sf::Vector2f(fishScale, fishScale));
    }

    newFish.spawnX = newFish.sprite->getPosition().x;
    newFish.sprite->setTextureRect(sf::IntRect({0, 0}, {286, 126}));
}

void UpdateSmallFishes(float dt)
{

    if (!stopSpawning)
    {
        spawnTimer += dt;
        if (spawnTimer >= spawnInterval)
        {
            spawnTimer = 0.f;
            StartSmallFish();
        }
    }

    for (int i = 0; i < MAX_SMALL_FISH; i++)
    {
        if (smallFishes[i].active)
        {
            SmallFish &fish = smallFishes[i];

            if (isEscapeMode && !fish.isFleeing)
            {
                fish.isFleeing = true;
                fish.velocity.x *= 3.0f;
                fish.velocity.y = 0;
                fish.canTurn = false;
            }

            if (fish.isFleeing)
            {
                fish.sprite->move(fish.velocity * dt);

                sf::Vector2f pos = fish.sprite->getPosition();
                if (pos.x < -200.f || pos.x > LevelWidth + 200.f)
                {
                    fish.active = false;
                    if (fish.sprite)
                    {
                        delete fish.sprite;
                        fish.sprite = nullptr;
                    }
                }
                continue;
            }

            if (fish.isTurning)
            {
                fish.animTimer += dt;
                if (fish.animTimer >= 0.15f)
                {
                    fish.animTimer = 0.f;
                    fish.currentFrame++;
                    if (fish.currentFrame > 5)
                        fish.currentFrame = 0;
                    fish.sprite->setTextureRect(sf::IntRect({fish.currentFrame * 286, 126}, {286, 126}));
                }

                fish.turnTimer += dt;
                if (fish.turnTimer >= 0.7f)
                {
                    fish.isTurning = false;
                    fish.currentFrame = 0;
                    fish.animTimer = 0.f;
                    fish.sprite->setTextureRect(sf::IntRect({0, 0}, {286, 126}));

                    fish.velocity.x = -fish.originalSpeedX;
                    fish.originalSpeedX = fish.velocity.x;

                    fish.velocity.y = static_cast<float>(rand() % 41 - 20);

                    float fishScale = std::abs(fish.sprite->getScale().x);
                    if (fish.velocity.x > 0)
                        fish.sprite->setScale(sf::Vector2f(-fishScale, fishScale));
                    else
                        fish.sprite->setScale(sf::Vector2f(fishScale, fishScale));

                    fish.hasTurned = true;
                }
            }
            else
            {
                fish.verticalTimer -= dt;
                if (fish.verticalTimer <= 0.f)
                {
                    fish.velocity.y = static_cast<float>(rand() % 101 - 50);
                    fish.verticalTimer = 1.0f + static_cast<float>(rand() % 300) / 100.0f;
                }

                fish.sprite->move(fish.velocity * dt);
                sf::Vector2f pos = fish.sprite->getPosition();

                float hudCeiling = view.getCenter().y - 300.f + 90.f;
                float topMargin = hudCeiling + 20.f;

                if (pos.y < topMargin)
                {
                    fish.velocity.y = std::abs(fish.velocity.y); // Force them to bounce down
                    if (fish.velocity.y < 10.f)
                        fish.velocity.y = 40.0f; // Give them a kick down
                    fish.verticalTimer = 2.0f;
                }
                else if (pos.y > LevelHeight - 50.0f)
                {
                    fish.velocity.y = -40.0f;
                    fish.verticalTimer = 2.0f;
                }

                if (fish.canTurn && !fish.hasTurned)
                {
                    float distanceTraveled = std::abs(pos.x - fish.spawnX);
                    bool shouldTurn = false;
                    if (distanceTraveled >= 150.f)
                    {
                        bool isSafeFromBorders = (pos.x > 150.f && pos.x < LevelWidth - 150.f);
                        if (isSafeFromBorders)
                        {
                            fish.timeToNextTurn -= dt;
                            if (fish.timeToNextTurn <= 0.f)
                                shouldTurn = true;
                        }
                    }

                    if (shouldTurn)
                    {
                        fish.isTurning = true;
                        fish.turnTimer = 0.f;
                        fish.currentFrame = 0;
                        fish.animTimer = 0.f;
                    }
                }

                fish.animTimer += dt;
                if (fish.animTimer >= 0.15f)
                {
                    fish.animTimer = 0.f;
                    fish.currentFrame++;
                    if (fish.currentFrame > 5)
                        fish.currentFrame = 0;
                    fish.sprite->setTextureRect(sf::IntRect({fish.currentFrame * 286, 0}, {286, 126}));
                }
            }

            sf::Vector2f finalPos = fish.sprite->getPosition();
            if (finalPos.x < -200.f || finalPos.x > LevelWidth + 200.f)
            {
                fish.active = false;
                if (fish.sprite)
                {
                    delete fish.sprite;
                    fish.sprite = nullptr;
                }
            }
        }
    }
}

void DrawSmallFishes(sf::RenderWindow &window)
{
    for (int i = 0; i < MAX_SMALL_FISH; i++)
    {
        if (smallFishes[i].active && smallFishes[i].sprite)
        {
            window.draw(*smallFishes[i].sprite);
        }
    }
}

void StartMediumFish()
{
    int freeIndex = -1;
    for (int i = 0; i < MAX_MEDIUM_FISH; i++)
    {
        if (!mediumFishes[i].active)
        {
            freeIndex = i;
            break;
        }
    }
    if (freeIndex == -1)
        return;

    MediumFish &fish = mediumFishes[freeIndex];
    fish.sprite = new sf::Sprite(mediumFishTexture);

    float scale = 0.5f;
    fish.sprite->setOrigin({(float)MEDIUM_FRAME_W / 2.f, (float)MEDIUM_FRAME_H / 2.f});

    int side = rand() % 2;
    float hudCeiling = view.getCenter().y - 300.f + 90.f;
    float minY = hudCeiling + 40.f; // Extra 40 for their larger body
    float maxY = LevelHeight - 80.f;
    if (maxY <= minY)
        maxY = minY + 10.f;
    float randomY = minY + (rand() % (int)(maxY - minY));
    float speed = 80.f;
    float randomYSpeed = static_cast<float>(rand() % 61 - 30);

    fish.canTurn = (rand() % 4 == 0);
    fish.hasTurned = false;

    fish.timeToNextTurn = 5.f + static_cast<float>(rand() % 350) / 100.0f;

    if (side == 0)
    {
        fish.sprite->setPosition({-100.f, randomY});
        fish.velocity = {speed, randomYSpeed};
        fish.facingRight = true;
        fish.sprite->setScale({-scale, scale});
    }
    else
    {
        fish.sprite->setPosition({LevelWidth + 100.f, randomY});
        fish.velocity = {-speed, randomYSpeed};
        fish.facingRight = false;
        fish.sprite->setScale({scale, scale});
    }

    fish.spawnX = fish.sprite->getPosition().x;
    fish.active = true;
    fish.isFleeing = false;
    fish.state = 1;
    fish.currentFrame = 0;
    fish.animTimer = 0.f;
    fish.sprite->setTextureRect(sf::IntRect({0, 1 * MEDIUM_FRAME_H}, {MEDIUM_FRAME_W, MEDIUM_FRAME_H}));
}

void UpdateMediumFishes(float dt)
{
    if (!stopSpawning)
    {
        mediumSpawnTimer += dt;
        if (mediumSpawnTimer >= mediumSpawnInterval)
        {
            mediumSpawnTimer = 0.f;
            StartMediumFish();
        }
    }

    for (int i = 0; i < MAX_MEDIUM_FISH; i++)
    {
        if (!mediumFishes[i].active)
            continue;
        MediumFish &fish = mediumFishes[i];

        // Escape Mode Logic
        if (isEscapeMode && !fish.isFleeing)
        {
            fish.isFleeing = true;
            fish.velocity.x *= 2.4f;
            fish.velocity.y = 0;
            fish.state = 1;
            fish.canTurn = false;
        }
        if (fish.isFleeing)
        {
            fish.sprite->move(fish.velocity * dt);
            sf::Vector2f pos = fish.sprite->getPosition();
            if (pos.x < -300.f || pos.x > LevelWidth + 300.f)
            {
                fish.active = false;
                if (fish.sprite)
                {
                    delete fish.sprite;
                    fish.sprite = nullptr;
                }
            }
            continue;
        }

        // Movement Logic
        if (fish.state == 1)
        {
            fish.sprite->move(fish.velocity * dt);
            fish.verticalTimer -= dt;
            if (fish.verticalTimer <= 0.f)
            {
                fish.velocity.y = static_cast<float>(rand() % 61 - 30);
                fish.verticalTimer = 1.5f + static_cast<float>(rand() % 400) / 100.0f;
            }
            sf::Vector2f pos = fish.sprite->getPosition();
            if (pos.y < 80.0f)
                fish.velocity.y = 30.0f;
            else if (pos.y > LevelHeight - 80.0f)
                fish.velocity.y = -30.0f;
        }
        Vector2f p = fish.sprite->getPosition();
        if (p.x < -300.f || p.x > LevelWidth + 300.f)
        {
            fish.active = false;
            delete fish.sprite;
            fish.sprite = nullptr;
            continue;
        }

        // Turning Logic
        if (fish.state == 1 && fish.canTurn && !fish.hasTurned)
        {
            sf::Vector2f pos = fish.sprite->getPosition();

            float turnDistance = 250.f;

            bool outOfBounds = (pos.x < turnDistance && fish.velocity.x < 0) || (pos.x > LevelWidth - turnDistance && fish.velocity.x > 0);

            if (outOfBounds)
            {
                fish.state = 2;
                fish.currentFrame = 0;
                fish.animTimer = 0.f;
                fish.hasTurned = true;
            }
            else
            {
                if (fish.timeToNextTurn > 0.f)
                    fish.timeToNextTurn -= dt;
                if (fish.timeToNextTurn <= 0.f)
                {
                    fish.state = 2;
                    fish.currentFrame = 0;
                    fish.animTimer = 0.f;
                    fish.hasTurned = true;
                }
            }
        }

        // Animation Logic
        fish.animTimer += dt;
        float animSpeed = (fish.state == 0) ? 0.06f : ((fish.state == 2) ? 0.08f : 0.12f);

        if (fish.animTimer >= animSpeed)
        {
            fish.animTimer = 0.f;
            fish.currentFrame++;

            if (fish.state == 0 && fish.currentFrame >= MEDIUM_FRAMES_EAT)
            {
                fish.currentFrame = 0;
                fish.state = 1;
            } // Finish Eating -> Swim
            else if (fish.state == 2 && fish.currentFrame >= MEDIUM_FRAMES_TURN)
            {
                fish.currentFrame = 0;
                fish.state = 1;
                fish.velocity.x = -fish.velocity.x;
                float scale = std::abs(fish.sprite->getScale().x);
                fish.sprite->setScale({(fish.velocity.x > 0 ? -scale : scale), scale});
                fish.spawnX = fish.sprite->getPosition().x;
            }
            else if (fish.state == 1 && fish.currentFrame >= MEDIUM_FRAMES_SWIM)
            {
                fish.currentFrame = 0;
            }
        }

        // === Interaction Logic (Eating & Death) ===
        if (fish.state == 1)
        {

            // 1. Medium Fish Eats Small Fish
            for (int j = 0; j < MAX_SMALL_FISH; j++)
            {
                if (smallFishes[j].active && smallFishes[j].sprite)
                {
                    float dx = smallFishes[j].sprite->getPosition().x - fish.sprite->getPosition().x;
                    float dy = smallFishes[j].sprite->getPosition().y - fish.sprite->getPosition().y;
                    float dist = std::sqrt(dx * dx + dy * dy);

                    if (dist < 40.f)
                    { // Eating Range
                      

                        smallFishes[j].active = false;
                        delete smallFishes[j].sprite;
                        smallFishes[j].sprite = nullptr;

                        // Play Eat Animation
                        fish.state = 0;
                        fish.currentFrame = 0;
                        fish.animTimer = 0.f;
                        break; // Eat only one
                    }
                }
            }

            // 2. Medium Fish Eats Player
            bool mediumShrunk = std::abs(mediumFishes[i].sprite->getScale().y) <= 0.21f;

            if (playerLevel < 2 && !mediumShrunk && !isPlayerDead && !isInvincible)
            {
                // Mouth Check
                sf::Vector2f fishDir(1.f, 0.f);
                if (fish.velocity.x != 0)
                    fishDir.x = (fish.velocity.x > 0) ? 1.f : -1.f;
                float predatorMouthOffset = 60.f;
                sf::Vector2f predatorMouthPos = fish.sprite->getPosition();
                predatorMouthPos.x += fishDir.x * predatorMouthOffset;

                float dx = sprPlayerall.getPosition().x - predatorMouthPos.x;
                float dy = sprPlayerall.getPosition().y - predatorMouthPos.y;
                float distance = std::sqrt(dx * dx + dy * dy);

                if (distance < 35.f)
                {
                    fish.state = 0;
                    fish.currentFrame = 0;
                    fish.animTimer = 0.f;
                    lives--;

                    if (lives > 0)
                    {
                        levelsound.stop();    
                        PlayergotEaten.play(); 
                        dieSound.play();       
                    }
                    else
                    {
                        levelsound.stop(); 
                        PlayergotEaten.play();
                        isGameOver = true; 
                        dieSound.play();   
                    }

                    isPlayerDead = true;
                    respawnClock.restart();
                    dieSound.play(); 
                    showSorryAnimation = true;
                    sorryExploded = false;
                    sorryTimer = 0.f;
                    for (int k = 0; k < 5; k++)
                        sorryLetterScales[k] = 0.f;
                }
            }
        }

        // Out of Bounds
        sf::Vector2f pos = fish.sprite->getPosition();
        float hudCeiling = view.getCenter().y - 300.f + 90.f;

        if (pos.y < hudCeiling + 40.f)
        {
            fish.velocity.y = std::abs(fish.velocity.y);
            if (fish.velocity.y < 10.f)
                fish.velocity.y = 30.0f;
        }
        else if (pos.y > LevelHeight - 80.0f)
        {
            fish.velocity.y = -30.0f;
        }

        int row = fish.state;
        fish.sprite->setTextureRect(sf::IntRect({fish.currentFrame * MEDIUM_FRAME_W, row * MEDIUM_FRAME_H}, {MEDIUM_FRAME_W, MEDIUM_FRAME_H}));
    }
}

void DrawMediumFishes(sf::RenderWindow &window)
{
    for (int i = 0; i < MAX_MEDIUM_FISH; i++)
    {
        if (mediumFishes[i].active && mediumFishes[i].sprite)
        {
            window.draw(*mediumFishes[i].sprite);
        }
    }
}

void StartLargeFish()
{
    int freeIndex = -1;
    for (int i = 0; i < MAX_LARGE_FISH; i++)
    {
        if (!largeFishes[i].active)
        {
            freeIndex = i;
            break;
        }
    }
    if (freeIndex == -1)
        return;

    LargeFish &fish = largeFishes[freeIndex];
    fish.sprite = new sf::Sprite(largeFishTexture);

    float scale = 0.9f;
    fish.sprite->setOrigin({(float)LARGE_FRAME_W / 2.f, (float)LARGE_FRAME_H / 2.f});

    int side = rand() % 2;
    float hudCeiling = view.getCenter().y - 300.f + 90.f;
    float minY = hudCeiling + 60.f; // Extra 60 for their huge body
    float maxY = LevelHeight - 120.f;
    if (maxY <= minY)
        maxY = minY + 10.f;
    float randomY = minY + (rand() % (int)(maxY - minY));
    float speed = 120.f;
    float randomYSpeed = static_cast<float>(rand() % 61 - 30);

    fish.canTurn = (rand() % 4 == 0);
    fish.hasTurned = false;

    fish.timeToNextTurn = 4.0f + static_cast<float>(rand() % 300) / 100.0f;

    if (side == 0)
    {
        fish.sprite->setPosition({-100.f, randomY});
        fish.velocity = {speed, randomYSpeed};
        fish.facingRight = true;
        fish.sprite->setScale({-scale, scale});
    }
    else
    {
        fish.sprite->setPosition({LevelWidth + 100.f, randomY});
        fish.velocity = {-speed, randomYSpeed};
        fish.facingRight = false;
        fish.sprite->setScale({scale, scale});
    }

    fish.spawnX = fish.sprite->getPosition().x;
    fish.active = true;
    fish.isFleeing = false;
    fish.state = 1;
    fish.currentFrame = 0;
    fish.animTimer = 0.f;
    fish.sprite->setTextureRect(sf::IntRect({0, 1 * LARGE_FRAME_H}, {LARGE_FRAME_W, LARGE_FRAME_H}));
}

void UpdateLargeFishes(float dt)
{
    if (!stopSpawning)
    {
        largeSpawnTimer += dt;
        if (largeSpawnTimer >= largeSpawnInterval)
        {
            largeSpawnTimer = 0.f;
            StartLargeFish();
        }
    }

    for (int i = 0; i < MAX_LARGE_FISH; i++)
    {
        if (!largeFishes[i].active)
            continue;
        LargeFish &fish = largeFishes[i];

        // Escape Mode
        if (isEscapeMode && !fish.isFleeing)
        {
            fish.isFleeing = true;
            fish.velocity.x *= 3.0f;
            fish.velocity.y = 0;
            fish.state = 1;
            fish.canTurn = false;
        }
        if (fish.isFleeing)
        {
            fish.sprite->move(fish.velocity * dt);
            sf::Vector2f pos = fish.sprite->getPosition();
            if (pos.x < -400.f || pos.x > LevelWidth + 400.f)
            {
                fish.active = false;
                if (fish.sprite)
                {
                    delete fish.sprite;
                    fish.sprite = nullptr;
                }
            }
            continue;
        }

        // Movement
        if (fish.state == 1)
        {
            fish.sprite->move(fish.velocity * dt);
            fish.verticalTimer -= dt;
            if (fish.verticalTimer <= 0.f)
            {
                fish.velocity.y = static_cast<float>(rand() % 41 - 20);
                fish.verticalTimer = 2.0f + static_cast<float>(rand() % 500) / 100.0f;
            }
            sf::Vector2f pos = fish.sprite->getPosition();
            if (pos.y < 120.0f)
                fish.velocity.y = 20.0f;
            else if (pos.y > LevelHeight - 120.0f)
                fish.velocity.y = -20.0f;
        }
        Vector2f p = fish.sprite->getPosition();
        if (p.x < -400.f || p.x > LevelWidth + 400.f)
        {
            fish.active = false;
            delete fish.sprite;
            fish.sprite = nullptr;
            continue;
        }

        // Turning
        if (fish.state == 1 && fish.canTurn && !fish.hasTurned)
        {
            sf::Vector2f pos = fish.sprite->getPosition();


            float turnDistance = 325.f;

            bool outOfBounds = (pos.x < turnDistance && fish.velocity.x < 0) || (pos.x > LevelWidth - turnDistance && fish.velocity.x > 0);

            if (outOfBounds)
            {
                fish.state = 2;
                fish.currentFrame = 0;
                fish.animTimer = 0.f;
                fish.hasTurned = true;
            }
            else
            {
                if (fish.timeToNextTurn > 0.f)
                    fish.timeToNextTurn -= dt;
                if (fish.timeToNextTurn <= 0.f)
                {
                    fish.state = 2;
                    fish.currentFrame = 0;
                    fish.animTimer = 0.f;
                    fish.hasTurned = true;
                }
            }
        }

        // Animation
        fish.animTimer += dt;
        float animSpeed = (fish.state == 0) ? 0.06f : ((fish.state == 2) ? 0.08f : 0.1f);
        if (fish.animTimer >= animSpeed)
        {
            fish.animTimer = 0.f;
            fish.currentFrame++;
            if (fish.state == 0 && fish.currentFrame >= LARGE_FRAMES_EAT)
            {
                fish.currentFrame = 0;
                fish.state = 1;
            } // Finish Eat
            else if (fish.state == 2 && fish.currentFrame >= LARGE_FRAMES_TURN)
            {
                fish.currentFrame = 0;
                fish.state = 1;
                fish.velocity.x = -fish.velocity.x;
                float scale = std::abs(fish.sprite->getScale().x);
                fish.sprite->setScale({(fish.velocity.x > 0 ? -scale : scale), scale});
                fish.spawnX = fish.sprite->getPosition().x;
            }
            else if (fish.state == 1 && fish.currentFrame >= LARGE_FRAMES_SWIM)
            {
                fish.currentFrame = 0;
            }
        }

        // === Interaction Logic ===
        if (fish.state == 1)
        {

            // 1. Large Fish Eats Medium Fish
            for (int j = 0; j < MAX_MEDIUM_FISH; j++)
            {
                if (mediumFishes[j].active && mediumFishes[j].sprite)
                {
                    float dx = mediumFishes[j].sprite->getPosition().x - fish.sprite->getPosition().x;
                    float dy = mediumFishes[j].sprite->getPosition().y - fish.sprite->getPosition().y;
                    float dist = std::sqrt(dx * dx + dy * dy);

                    if (dist < 50.f)
                    { // Eating Range

                        mediumFishes[j].active = false;
                        delete mediumFishes[j].sprite;
                        mediumFishes[j].sprite = nullptr;

                        fish.state = 0;
                        fish.currentFrame = 0;
                        fish.animTimer = 0.f;
                        break;
                    }
                }
            }

            // 2. Large Fish Eats Small Fish
            for (int j = 0; j < MAX_SMALL_FISH; j++)
            {
                if (smallFishes[j].active && smallFishes[j].sprite)
                {
                    float dx = smallFishes[j].sprite->getPosition().x - fish.sprite->getPosition().x;
                    float dy = smallFishes[j].sprite->getPosition().y - fish.sprite->getPosition().y;
                    float dist = std::sqrt(dx * dx + dy * dy);

                    if (dist < 50.f)
                    {

                        smallFishes[j].active = false;
                        delete smallFishes[j].sprite;
                        smallFishes[j].sprite = nullptr;

                        fish.state = 0;
                        fish.currentFrame = 0;
                        fish.animTimer = 0.f;
                        break;
                    }
                }
            }

            // 3. Large Fish Eats Player
            bool largeShrunk = std::abs(largeFishes[i].sprite->getScale().y) <= 0.21f;

            if (playerLevel < 3 && !largeShrunk && !isPlayerDead && !isInvincible)
            {
                sf::Vector2f fishDir(1.f, 0.f);
                if (fish.velocity.x != 0)
                    fishDir.x = (fish.velocity.x > 0) ? 1.f : -1.f;

                float predatorMouthOffset = 100.f;
                sf::Vector2f predatorMouthPos = fish.sprite->getPosition();
                predatorMouthPos.x += fishDir.x * predatorMouthOffset;

                float dx = sprPlayerall.getPosition().x - predatorMouthPos.x;
                float dy = sprPlayerall.getPosition().y - predatorMouthPos.y;
                float distance = std::sqrt(dx * dx + dy * dy);

                if (distance < 40.f)
                {
                    fish.state = 0;
                    fish.currentFrame = 0;
                    fish.animTimer = 0.f;
                    lives--;


                    if (lives > 0)
                    {
                        levelsound.stop();   
                        PlayergotEaten.play(); 
                    }
                    else
                    {
                        levelsound.stop();
                        PlayergotEaten.play();
                        isGameOver = true; 
                    }

                    isPlayerDead = true;
                    respawnClock.restart();
                    dieSound.play(); 
                    showSorryAnimation = true;
                    sorryExploded = false;
                    sorryTimer = 0.f;
                    for (int k = 0; k < 5; k++)
                        sorryLetterScales[k] = 0.f;
                }
            }
        }

        sf::Vector2f pos = fish.sprite->getPosition();
        float hudCeiling = view.getCenter().y - 300.f + 90.f;

        if (pos.y < hudCeiling + 60.f)
        {
            fish.velocity.y = std::abs(fish.velocity.y);
            if (fish.velocity.y < 10.f)
                fish.velocity.y = 20.0f;
        }
        else if (pos.y > LevelHeight - 120.0f)
        {
            fish.velocity.y = -20.0f;
        }

        int row = fish.state;
        fish.sprite->setTextureRect(sf::IntRect({fish.currentFrame * LARGE_FRAME_W, row * LARGE_FRAME_H}, {LARGE_FRAME_W, LARGE_FRAME_H}));
    }
}

void DrawLargeFishes(sf::RenderWindow &window)
{
    for (int i = 0; i < MAX_LARGE_FISH; i++)
    {
        if (largeFishes[i].active && largeFishes[i].sprite)
        {
            window.draw(*largeFishes[i].sprite);
        }
    }
}


void StartGameBubble(float x, float y, bool isAction)
{
    for (int i = 0; i < MAX_GAME_BUBBLES; i++)
    {
        if (!gameBubbles[i].active)
        {
            gameBubbles[i].active = true;
            gameBubbles[i].x = x;
            gameBubbles[i].y = y;
            gameBubbles[i].alpha = 255.f;
            gameBubbles[i].isAction = isAction;

            if (isAction)
            {
                
                gameBubbles[i].vx = getRandom(-60.f, 60.f);   
                gameBubbles[i].vy = getRandom(-100.f, -40.f); 
            }
            else
            {
               
                gameBubbles[i].vx = getRandom(-120.f, -60.f);
                gameBubbles[i].vy = getRandom(-10.f, 10.f);
            }
            break;
        }
    }
}


void UpdateGameBubbles(float dt)
{

    gameBubbleSpawnTimer += dt;


    if (gameBubbleSpawnTimer > 3.0f)
    {
        gameBubbleSpawnTimer = 0.f;


        float spawnX = view.getCenter().x + view.getSize().x / 2.f + 50.f;
        float spawnY = getRandom(50.f, LevelHeight - 50.f);
        StartGameBubble(spawnX, spawnY, false);
    }


    for (int i = 0; i < MAX_GAME_BUBBLES; i++)
    {
        if (gameBubbles[i].active)
        {
            gameBubbles[i].x += gameBubbles[i].vx * dt;
            gameBubbles[i].y += gameBubbles[i].vy * dt;

            if (gameBubbles[i].isAction)
            {
                gameBubbles[i].alpha -= 350.f * dt;
                if (gameBubbles[i].alpha <= 0)
                    gameBubbles[i].active = false;
            }
            else
            {

                if (gameBubbles[i].x < view.getCenter().x - view.getSize().x / 2.f - 100.f)
                {
                    gameBubbles[i].active = false;
                }
            }
        }
    }
}


void DrawGameBubbles()
{
    for (int i = 0; i < MAX_GAME_BUBBLES; i++)
    {
        if (gameBubbles[i].active)
        {
            bubbleSmallSprite.setPosition({gameBubbles[i].x, gameBubbles[i].y});
            bubbleSmallSprite.setColor(Color(255, 255, 255, (uint8_t)gameBubbles[i].alpha));


            float s = gameBubbles[i].isAction ? 0.5f : 0.6f;

            bubbleSmallSprite.setScale({s, s});
            window.draw(bubbleSmallSprite);
        }
    }
}

// ==========================================
//           Mermaid Event Logic
// ==========================================

void StartMermaidEvent()
{

    if (!mermaidTex.loadFromFile("Assets/fish/mermaid/mermaid.png"))
    {
        cout << "Error loading mermaid.png" << endl;
    }
    if (!starTexture.loadFromFile("Assets/bouns/starbubble1.png"))
    {
        cout << "Error loading starbubble1.png" << endl;
    }

    hasPlayedExitSound = false;

    mermaidTex.setSmooth(true);
    starTexture.setSmooth(true);

    mermaidSprite.setTexture(mermaidTex);
    mermaidSprite.setTextureRect(IntRect({0, 0}, {299, 88}));
    mermaidSprite.setScale({1.0f, 1.0f});
    mermaidSprite.setOrigin({149.5f, 44.f});

    float startX = view.getCenter().x + (view.getSize().x / 2.f) + 150.f;
    float startY = LevelHeight / 2.0f;

    mermaidSprite.setPosition({startX, startY});

    mermaidFrame = 0;
    mermaidAnimTimer = 0.0f;
    starSpawnTimer = 0.0f;
    isMermaidEventActive = true;
    mermaidFinished = false;
    levelsound.stop();
    mermaidevent.play();
    mermaidevent.setLooping(true); 
    for (int i = 0; i < MAX_STARS; i++)
        stars[i].active = false;
}

void UpdateMermaidEvent(float dt)
{
    if (!isMermaidEventActive)
        return;

    float moveSpeed = 250.0f;
    mermaidSprite.move({-moveSpeed * dt, 0});

    mermaidAnimTimer += dt;
    if (mermaidAnimTimer >= 0.08f)
    {
        mermaidAnimTimer = 0.0f;
        mermaidFrame++;

        if (mermaidFrame >= 20)
            mermaidFrame = 0;

        int frameWidth = 299;
        int frameHeight = 88;

        mermaidSprite.setTextureRect(IntRect({mermaidFrame * frameWidth, 0}, {frameWidth, frameHeight}));
    }

    mermaidSprite.setOrigin({149.5f, 44.f});

    starSpawnTimer += dt;
    if (starSpawnTimer >= 0.5f)
    {
        starSpawnTimer = 0.0f;
        for (int i = 0; i < MAX_STARS; i++)
        {
            if (!stars[i].active)
            {
                stars[i].active = true;

                if (stars[i].sprite == nullptr)
                {
                    stars[i].sprite = new Sprite(starTexture);
                }
                else
                {
                    stars[i].sprite->setTexture(starTexture);
                }

                stars[i].sprite->setScale({0.8f, 0.8f});
                stars[i].sprite->setOrigin({26, 25.5f});
                stars[i].sprite->setPosition(mermaidSprite.getPosition());
                stars[i].velocity = {0, 120.f};
                break;
            }
        }
    }

    for (int i = 0; i < MAX_STARS; i++)
    {
        if (stars[i].active && stars[i].sprite != nullptr)
        {
            stars[i].sprite->move(stars[i].velocity * dt);

            sf::Vector2f pPos = sprPlayerall.getPosition();
            sf::Vector2f sPos = stars[i].sprite->getPosition();
            float dist = sqrt(pow(pPos.x - sPos.x, 2) + pow(pPos.y - sPos.y, 2));

            if (dist < 50.f)
            {
                stars[i].active = false;
                starsEatenCount++; 
                score += 100 * multiplier;
                createScorePopup(sPos.x, sPos.y, 100 * multiplier);

                eatSound.play();

                if (currentState == TURN)
                {
                    pendingEat = true;
                }
                else
                {
                    currentState = EAT;
                    currentFrame = 0;
                    timer = 0.f;
                }
            }

            if (stars[i].sprite->getPosition().y > LevelHeight + 50.f)
            {
                stars[i].active = false;
            }
        }
    }

    float camLeft = view.getCenter().x - (view.getSize().x / 2.f);
    if (mermaidSprite.getPosition().x < camLeft - 200.f)
    {
        if (!mermaidFinished)
        {
            mermaidFinished = true;
            eventEndClock.restart();
        }
    }

    if (mermaidFinished)
    {
        float elapsed = eventEndClock.getElapsedTime().asSeconds();


        if (elapsed >= 1.0f && !showPerfectAnimation && !perfectExploded)
        {
            showPerfectAnimation = true;
            perfectTimer = 0.f;
            for (int k = 0; k < 7; k++)
                perfectLetterScales[k] = 0.f;
            mermaidevent.stop();
        }

        if (elapsed >= 2.0f && !hasPlayedExitSound)
        {
            StageClearSound.play();
            hasPlayedExitSound = true;
        }

        if (elapsed >= 6.0f)
        {

            addNewHighScore(userName, score, currentGamemode == CLASSIC);
            for (int i = 0; i < NumberOfUsers; i++)
            {
                if (players[i].name == CurUser)
                {
                    players[i].score = score;
                    break;
                }
            }

            if (currentGamemode == CLASSIC)
            {
                storyCarryScore = score;
                lastWonStoryLevel = selectedLevel;
            }
            else // TIMEATTACK
            {
                taCarryScore = score;
                lastWonTALevel = selectedLevel;

                for (int i = 0; i < NumberOfUsers; i++)
                {
                    if (players[i].name == CurUser)
                    {
                        players[i].score += score;
                        break;
                    }
                }
            }

            addNewHighScore(userName, score, currentGamemode == CLASSIC);

            if (currentGamemode == CLASSIC)
            {
                if (selectedLevel == 1)
                {
                    level2Unlocked = true; 
                }
                else if (selectedLevel == 2)
                {
                    level3Unlocked = true; 
                }
            }

            else if (currentGamemode == TIMEATTACK)
            {
                if (selectedLevel == 1)
                {
                    ta_level2Unlocked = true; 
                }
                else if (selectedLevel == 2)
                {
                    ta_level3Unlocked = true; 
                }
            }
            SaveGameData();
            EndLevel(); 

            isLevelRunning = false;
            goToMainMenuFromLevel = true;
        }
    }
}

void DrawMermaidEvent()
{
    if (!isMermaidEventActive)
        return;

    window.draw(mermaidSprite);

    for (int i = 0; i < MAX_STARS; i++)
    {
        if (stars[i].active && stars[i].sprite != nullptr)
        {
            window.draw(*stars[i].sprite); 
        }
    }
}

void StartLevelHud()
{
    frenzyProgress = 0;
    timeAttackTimer = 0.f;
    growthPercentage = 0.f;
    frenzyDecayAccumulator = 0.f;

    for (int i = 0; i < 3; i++)
    {
        fishIcons[i].normalTex.emplace(texTitle);
        fishIcons[i].shadowTex.emplace(texTitle);
        fishIcons[i].sprite.emplace(texTitle);
    }

    if (!texHudAbility.loadFromFile("Assets/HUD/hud_ability.png"))
        cout << "Missing ability.png\n";
    texHudAbility.setSmooth(true);

    if (!texTimerBg.loadFromFile("Assets/HUD/TimeAttack/timerbg.png"))
        cout << "Missing timerbg.png\n";
    texTimerBg.setSmooth(true);
    sprTimerBg.setTexture(texTimerBg, true);
    sprTimerBg.setPosition({WindowWidth / 2, 580.f});
    sprTimerBg.setOrigin({texTimerBg.getSize().x / 2.f, texTimerBg.getSize().y / 2.f});

    if (!timerFont.openFromFile("Assets/Fonts/Fedora.ttf"))
        cout << "Missing timer font\n";
    timerFont.setSmooth(true);

    txtTimer.setFont(timerFont);
    txtTimer.setCharacterSize(22);
    txtTimer.setFillColor(Color::White);
    txtTimer.setOutlineThickness(1.f);
    txtTimer.setOutlineColor(Color(0, 0, 0));

    if (!multiplierFont.openFromFile("Assets/Fonts/Barmeno.ttf"))
    {
        cout << "Could not load Barmeno font for multiplier!" << endl;
    }
    txtScore.setFont(multiplierFont);
    txtScore.setCharacterSize(30);               
    txtScore.setFillColor(Color(255, 230, 131)); 
    txtScore.setOutlineThickness(1.5f);
    txtScore.setOutlineColor(Color(60, 40, 0, 200));

    if (!fishIcons[0].normalTex->loadFromFile("Assets/HUD/FishIcons/menu_herring.png"))
        cout << "Missing menu_herring.png\n";
    if (!fishIcons[0].shadowTex->loadFromFile("Assets/HUD/FishIcons/menu_herring_shadow.png"))
        cout << "Missing menu_herring_shadow.png\n";

    if (!fishIcons[1].normalTex->loadFromFile("Assets/HUD/FishIcons/menu_cod.png"))
        cout << "Missing menu_cod.png\n";
    if (!fishIcons[1].shadowTex->loadFromFile("Assets/HUD/FishIcons/menu_cod_shadow.png"))
        cout << "Missing menu_cod_shadow.png\n";

    if (!fishIcons[2].normalTex->loadFromFile("Assets/HUD/FishIcons/menu_lionfish.png"))
        cout << "Missing menu_lionfish.png\n";
    if (!fishIcons[2].shadowTex->loadFromFile("Assets/HUD/FishIcons/menu_lionfish_shadow.png"))
        cout << "Missing menu_lionfish_shadow.png\n";

    if (!dangerIconTex.loadFromFile("Assets/HUD/FishIcons/menu_danger.png"))
        cout << "Missing menu_danger.png\n";

    static bool frenzyFontLoaded = false;
    if (!texHudTop.loadFromFile("Assets/HUD/hud_bgtop.jpg"))
        cout << "Missing hud_bgtop.jpg\n";
    if (!texHudBottom.loadFromFile("Assets/HUD/hud_bgbottom.png"))
        cout << "Missing hud_bgbottom.png\n";
    if (!texHudGrowth.loadFromFile("Assets/HUD/growth.png"))
        cout << "Missing hud_growthtext.png\n";
    if (!texHudGrowthMarker.loadFromFile("Assets/HUD/Screenshot.png"))
        cout << "Missing Screenshot.png\n";
    if (!frenzyFontLoaded)
    {
        if (!frenzyFont.openFromFile("Assets/Fonts/Fedora.ttf"))
            cout << "Missing Fedora.ttf\n";
        frenzyFont.setSmooth(true);
        frenzyFontLoaded = true;
    }

    string frenzyWord = "FRENZY!";

    texHudTop.setSmooth(true);
    texHudBottom.setSmooth(true);
    texHudGrowth.setSmooth(true);
    texHudGrowthMarker.setSmooth(true);

    txtMultiplier.setFont(multiplierFont);
    txtMultiplier.setCharacterSize(22);

    float topScale = WindowWidth / texHudTop.getSize().x;

    int currentSliceX = 0; 
    float currentDrawX = WindowWidth / 2.f - 80.f;
    float frenzyStartY = 5.f;

    for (int i = 0; i < 7; i++)
    {
        // Emplace creates the Text object and gives it the font, string, and size all at once!
        txtFrenzyLetters[i].emplace(frenzyFont, string(1, frenzyWord[i]), 27);

        // Add a nice dark outline so the colors pop off the wood
        txtFrenzyLetters[i]->setOutlineThickness(1.f);
        txtFrenzyLetters[i]->setOutlineColor(Color(119, 119, 118, 160));

        txtFrenzyLetters[i]->setPosition({currentDrawX + 26, frenzyStartY + 16});

        // Advance the X position by the width of the letter we just drew
        float letterWidth = txtFrenzyLetters[i]->getLocalBounds().size.x;
        currentDrawX += (letterWidth - 4.f);
    }

    float multX = currentDrawX + 283.f; 
    float multY = frenzyStartY + 3;    
    txtMultiplier.setPosition({multX, multY});

    for (int i = 0; i < 3; i++)
    {
        fishIcons[i].normalTex->setSmooth(true);
        fishIcons[i].shadowTex->setSmooth(true);
        fishIcons[i].sprite->setTexture(*fishIcons[i].normalTex, true);

        float xPos = 75.f + (i * 80.f);
        fishIcons[i].sprite->setPosition({xPos, 5.f});
        fishIcons[i].sprite->setScale({1.f, 1.f});
    }

    dangerIconTex.setSmooth(true);
    dangerIcon.setTexture(dangerIconTex, true);
    dangerIcon.setScale({0.85f, 0.85f});
    playerSize = 1;

    sprHudTop.setTexture(texHudTop, true);
    sprHudBottom.setTexture(texHudBottom, true);
    sprHudGrowth.setTexture(texHudGrowth, true);
    sprHudGrowthMarker1.setTexture(texHudGrowthMarker, true);
    sprHudGrowthMarker2.setTexture(texHudGrowthMarker, true);

    sprHudTop.setScale({topScale, topScale});
    sprHudTop.setPosition({0.f, 0.f});


    float botScale = WindowWidth / texHudBottom.getSize().x;
    sprHudBottom.setScale({botScale, botScale});
 
    sprHudBottom.setPosition({0.f, texHudTop.getSize().y * topScale});

    sprHudGrowth.setScale({topScale, topScale});
    sprHudGrowth.setPosition({10.f, 45.f});

    sprHudGrowthMarker1.setScale({topScale, topScale});
    sprHudGrowthMarker2.setScale({topScale, topScale});

    float barStartX = 165.f;    
    float barY = 48.f;           
    float barTotalWidth = 208.f; 

    // Calculate exactly what percentage of the total bar Marker 1 and Marker 2 should sit at
    float m1Ratio = static_cast<float>(currentLevelSettings.fishToGrowToLevel2) / currentLevelSettings.fishToWin;
    float m2Ratio = static_cast<float>(currentLevelSettings.fishToGrowToLevel3) / currentLevelSettings.fishToWin;

    sprHudGrowthMarker1.setPosition({barStartX + (barTotalWidth * m1Ratio), barY});
    sprHudGrowthMarker2.setPosition({barStartX + (barTotalWidth * m2Ratio), barY});

    growthBarFill.setFillColor(Color(229, 229, 25)); 
    growthBarFill.setPosition({barStartX - 56, barY + 5});

    sprHudAbility.setTexture(texHudAbility, true);
    sprHudAbility.setScale({topScale, topScale});
    sprHudAbility.setPosition({500.f, 45.f});
}

void UpdateLevelHud()
{

    Vector2f mousePos = window.mapPixelToCoords(Mouse::getPosition(window), uiView);

    while (const optional event = window.pollEvent())
    {
        if (event->is<Event::Closed>())
            window.close();

        if (const auto *resizeEvent = event->getIf<Event::Resized>())
        {
            view.setViewport(FloatRect({0.f, 0.f}, {1.f, 1.f}));
            window.setView(view);
        }

        if (auto mouseEvent = event->getIf<Event::MouseButtonReleased>())
        {
            if (mouseEvent->button == Mouse::Button::Left)
            {

                if (mousePos.x < 130.f && mousePos.y < 70.f)
                {
                    isLevelRunning = false;
                }
            }
        }

        if (const auto *keyPressed = event->getIf<Event::KeyPressed>())

            if (keyPressed->code == Keyboard::Key::Escape)
            {
                ShowPauseMenu();
                return;
            }
    }

    int currentTierBase = (multiplier - 1) * 7;
    int litLettersInCurrentTier = static_cast<int>((comboProgress * 7.0f) + 0.01f);
    frenzyProgress = currentTierBase + litLettersInCurrentTier;

    // Safety clamp just in case
    if (frenzyProgress > 35)
        frenzyProgress = 35;
    if (frenzyProgress < 0)
        frenzyProgress = 0;

    growthPercentage = (static_cast<float>(fishEatenCount) / currentLevelSettings.fishToWin) * 100.f;
    if (growthPercentage > 100.f)
        growthPercentage = 100.f;
    if (growthPercentage < 0.f)
        growthPercentage = 0.f;

 
    float barStartX = 108.f;
    float barY = 48.f;

    float barTotalWidth = 208.5f;


    float currentFillWidth = barTotalWidth * (growthPercentage / 100.f);
    growthBarFill.setSize({currentFillWidth, 12.f});
    growthBarFill.setPosition({barStartX, barY + 5.f});

    sprHudGrowthMarker1.setOrigin({sprHudGrowthMarker1.getLocalBounds().size.x / 2.f, 0.f});
    sprHudGrowthMarker2.setOrigin({sprHudGrowthMarker2.getLocalBounds().size.x / 2.f, 0.f});

    float m1Ratio = static_cast<float>(currentLevelSettings.fishToGrowToLevel2) / currentLevelSettings.fishToWin;
    float m2Ratio = static_cast<float>(currentLevelSettings.fishToGrowToLevel3) / currentLevelSettings.fishToWin;

    sprHudGrowthMarker1.setPosition({barStartX + (barTotalWidth * m1Ratio), barY});
    sprHudGrowthMarker2.setPosition({barStartX + (barTotalWidth * m2Ratio), barY});

    // === 3. SYNC PLAYER SIZE ICON ===
    playerSize = playerLevel;

    int tierIndex = frenzyProgress / 7;


    Color tierColors[] = {
        Color(47, 199, 36),  // 2: Green
        Color(247, 148, 28), // 3: Orange
        Color(74, 223, 255), // 4: Blue
        Color(234, 227, 33), // 5: Yellow
        Color(237, 33, 42)   // 6: Red
    };

    if (tierIndex == 0)
    {
        txtMultiplier.setString("");
    }
    else
    {

        std::stringstream ss;
        ss << (tierIndex + 1) << "x";
        txtMultiplier.setString(ss.str());

        int colorIdx = (tierIndex - 1);
        if (colorIdx > 4)
            colorIdx = 4;

        txtMultiplier.setFillColor(tierColors[colorIdx]);
        txtMultiplier.setOutlineThickness(1.5f);
        Color c = tierColors[colorIdx];
        txtMultiplier.setOutlineColor(Color(c.r * 0.4f, c.g * 0.4f, c.b * 0.4f));
    }
    txtScore.setString(formatScore(score));
    FloatRect scoreBounds = txtScore.getLocalBounds();
    // Set origin to the Top-Right corner of the text
    txtScore.setOrigin({scoreBounds.position.x + scoreBounds.size.x, 0.f});
    txtScore.setPosition({WindowWidth - 65.f, 2.f});
}

void DrawLevelHud(bool doClear)
{
    window.setView(uiView);

    if (doClear)
        window.clear(Color(20, 100, 160));

    window.draw(sprHudTop);
    window.draw(sprHudBottom);
    window.draw(sprHudGrowth);

    window.draw(growthBarFill);
    window.draw(sprHudAbility);

    window.draw(sprHudGrowthMarker1);
    window.draw(sprHudGrowthMarker2);
    if (txtMultiplier.getString() != "")
    {
        window.draw(txtMultiplier);
    }

    window.draw(txtScore);
    // --- DRAW LIVES ---
    Text txtLives(multiplierFont,to_string(lives), 22);
    txtLives.setFillColor(Color(255, 255, 255));
    txtLives.setOutlineThickness(1.5f);
    txtLives.setOutlineColor(Color(0, 0, 0, 200));
    txtLives.setPosition({ 760.f, 44.f });
    window.draw(txtLives);

    for (int i = 0; i < 3; i++)
    {
        fishIcons[i].canEat = (i < playerSize);

        if (fishIcons[i].canEat)
        {
            fishIcons[i].sprite->setTexture(*fishIcons[i].normalTex, true);
            window.draw(*fishIcons[i].sprite);
        }
        else
        {
            fishIcons[i].sprite->setTexture(*fishIcons[i].shadowTex, true);
            window.draw(*fishIcons[i].sprite);

            Vector2f pos = fishIcons[i].sprite->getPosition();
            dangerIcon.setPosition({pos.x + 37.f, pos.y + 30.f});
            window.draw(dangerIcon);
        }
    }

    // --- DRAW FRENZY! MULTIPLIER ---
    Color tierColors[5] = {
        Color(47, 199, 36),  // 0: Green
        Color(247, 148, 28), // 1: Orange
        Color(74, 223, 255), // 2: Blue
        Color(234, 227, 33), // 3: Yellow
        Color(237, 33, 42)   // 4: Red
    };

    for (int i = 0; i < 7; i++)
    {
        if (txtFrenzyLetters[i])
        {
            if (frenzyProgress <= i)
            {
                txtFrenzyLetters[i]->setFillColor(Color(157, 157, 157));
            }
            else
            {
                int tierIndex = (frenzyProgress - i - 1) / 7;
                if (tierIndex > 4)
                    tierIndex = 4;
                txtFrenzyLetters[i]->setFillColor(tierColors[tierIndex]);
            }

            Color c = txtFrenzyLetters[i]->getFillColor();
            txtFrenzyLetters[i]->setOutlineThickness(1.5f), txtFrenzyLetters[i]->setOutlineColor(Color(c.r * 0.4f, c.g * 0.4f, c.b * 0.4f));

            window.draw(*txtFrenzyLetters[i]);
        }
    }

    if (isTimeAttackMode)
    {
        window.draw(sprTimerBg);

        int minutes = static_cast<int>(remainingTime) / 60;
        int seconds = static_cast<int>(remainingTime) % 60;

        string timeStr = to_string(minutes) + ":" + (seconds < 10 ? "0" : "") + to_string(seconds);
        txtTimer.setString(timeStr);

        if (remainingTime <= 10.f)
            txtTimer.setFillColor(Color::Red);
        else
            txtTimer.setFillColor(Color::White);

        FloatRect b = txtTimer.getLocalBounds();
        txtTimer.setOrigin({b.position.x + b.size.x / 2.f, b.position.y + b.size.y / 2.f});
        txtTimer.setPosition({sprTimerBg.getPosition().x, sprTimerBg.getPosition().y - 3.f});

        window.draw(txtTimer);
    }
}

bool ShowEndGameMenu()
{
    static bool loaded = false;
    static Texture panelTex, titleTex;
    static Texture shortBtnNormalTex, shortBtnHoverTex;
    static Texture longBtnNormalTex, longBtnHoverTex;
    static Font endFont;

    if (!loaded)
    {
        if (!endFont.openFromFile("Assets/Fonts/BarmenoBold.otf"))
            cout << "Missing BarmenoBold.otf\n";

        if (!panelTex.loadFromFile("Assets/Pause/end_game.png"))
            cout << "Missing end_game.png\n";
        if (!titleTex.loadFromFile("Assets/Pause/quitgame_icon.png"))
            cout << "Missing quitgame_icon.png\n";
        if (!shortBtnNormalTex.loadFromFile("Assets/Pause/shell_smallbtn210_normal.png"))
            cout << "Missing shell_largebtn152_normal.png\n";
        if (!shortBtnHoverTex.loadFromFile("Assets/Pause/shell_smallbtn210_high.png"))
            cout << "Missing shell_largebtn152_high.png\n";
        if (!longBtnNormalTex.loadFromFile("Assets/Pause/shell_smallbtn266_normal.png"))
            cout << "Missing shell_largebtn225_normal.png\n";
        if (!longBtnHoverTex.loadFromFile("Assets/Pause/shell_smallbtn266_high.png"))
            cout << "Missing shell_largebtn225_high.png\n";

        endFont.setSmooth(true);
        panelTex.setSmooth(true);
        titleTex.setSmooth(true);
        shortBtnNormalTex.setSmooth(true);
        shortBtnHoverTex.setSmooth(true);
        longBtnNormalTex.setSmooth(true);
        longBtnHoverTex.setSmooth(true);

        loaded = true;
    }

    auto centerText = [&](Text &txt, float x, float y)
    {
        FloatRect b = txt.getLocalBounds();
        txt.setOrigin({b.position.x + b.size.x / 2.f, b.position.y + b.size.y / 2.f});
        txt.setPosition({x, y});
    };

    auto setupSprite = [&](Sprite &s, const Texture &tex, Vector2f pos, Vector2f scale)
    {
        s.setTexture(tex, true);
        FloatRect b = s.getLocalBounds();
        s.setOrigin({b.size.x / 2.f, b.size.y / 2.f});
        s.setPosition(pos);
        s.setScale(scale);
    };

    const Color greenText(160, 211, 74);
    const Color yellowHover(255, 241, 74);
    const Color whiteText(236, 242, 244);
    const Color outlineColor(100, 78, 42);

    const Vector2f panelPos = {WindowWidth / 2.f, 322.f};
    const Vector2f panelScale = {1.f, 1.f};

    const Vector2f titlePos = {WindowWidth / 2.f, 180.f};
    const Vector2f titleScale = {0.18f, 0.18f};

    const Vector2f shortScale = {1.f, 1.f};
    const Vector2f longScale = {1.f, 1.f};

    const Vector2f savePos = {WindowWidth / 2.f, 290.f};
    const Vector2f quitPos = {WindowWidth / 2.f, 340.f};
    const Vector2f cancelPos = {WindowWidth / 2.f, 405.f};

    screenDarkener.setSize(Vector2f(WindowWidth, WindowHeight));
    screenDarkener.setPosition({0.f, 0.f});
    screenDarkener.setFillColor(Color(0, 0, 0, 120));

    Clock frameClock;
    while (window.isOpen() && isLevelRunning)
    {
        float dt = frameClock.restart().asSeconds();

        while (const optional event = window.pollEvent())
        {
            if (event->is<Event::Closed>())
            {
                window.close();
                return false;
            }

            if (const auto *resizeEvent = event->getIf<Event::Resized>())
            {
                // view.setViewport(FloatRect({0.f, 0.f}, {1.f, 1.f}));
                // window.setView(view);
            }

            if (const auto *keyPressed = event->getIf<Event::KeyPressed>())
            {
                if (keyPressed->code == Keyboard::Key::Escape)
                    return false;
            }

            if (const auto *mouseEvent = event->getIf<Event::MouseButtonReleased>())
            {
                if (mouseEvent->button == Mouse::Button::Left)
                {
                    Vector2f mousePos = window.mapPixelToCoords(Mouse::getPosition(window), uiView);

                    Sprite saveBtn(shortBtnNormalTex), quitBtn(longBtnNormalTex), cancelBtn(shortBtnNormalTex);

                    setupSprite(saveBtn, shortBtnNormalTex, savePos, shortScale);
                    setupSprite(quitBtn, longBtnNormalTex, quitPos, longScale);
                    setupSprite(cancelBtn, shortBtnNormalTex, cancelPos, shortScale);

                    if (saveBtn.getGlobalBounds().contains(mousePos))
                    {
                        addNewHighScore(userName, score, currentGamemode == CLASSIC);
                        levelsound.stop();      
                        WaveSound.stop();      
                        mainmenumusic.play();    
                        mainmenumusic.setLooping(true);
                        SaveGameData();
                        goToMainMenuFromLevel = true;
                        isLevelRunning = false;
                        return true;
                    }

                    if (quitBtn.getGlobalBounds().contains(mousePos))
                    {
                        levelsound.stop();      
                        WaveSound.stop();      
                        mainmenumusic.play();    
                        mainmenumusic.setLooping(true);
                        isLevelRunning = false;
                        goToMainMenuFromLevel = true;
                        return true;
                    }

                    if (cancelBtn.getGlobalBounds().contains(mousePos))
                    {
                        return false;
                    }
                }
            }
        }

        Vector2f mousePos = window.mapPixelToCoords(Mouse::getPosition(window), uiView);

        Sprite saveBtn(shortBtnNormalTex), quitBtn(longBtnNormalTex), cancelBtn(shortBtnNormalTex);

        setupSprite(saveBtn, shortBtnNormalTex, savePos, shortScale);
        setupSprite(quitBtn, longBtnNormalTex, quitPos, longScale);
        setupSprite(cancelBtn, shortBtnNormalTex, cancelPos, shortScale);

        bool saveHover = saveBtn.getGlobalBounds().contains(mousePos);
        bool quitHover = quitBtn.getGlobalBounds().contains(mousePos);
        bool cancelHover = cancelBtn.getGlobalBounds().contains(mousePos);

        static bool wasSaveHover = false, wasQuitHover = false, wasCancelHover = false;
        if ((saveHover && !wasSaveHover) || (quitHover && !wasQuitHover) || (cancelHover && !wasCancelHover))
        {
            if (isSoundEnabled)
                buttonpressedsound.play();
        }
        wasSaveHover = saveHover;
        wasQuitHover = quitHover;
        wasCancelHover = cancelHover;

        if (saveHover)
            setupSprite(saveBtn, shortBtnHoverTex, savePos, shortScale);
        if (quitHover)
            setupSprite(quitBtn, longBtnHoverTex, quitPos, longScale);
        if (cancelHover)
            setupSprite(cancelBtn, shortBtnHoverTex, cancelPos, shortScale);

        window.clear(Color(20, 100, 160));
        Drawbglevel();                      
        window.setMouseCursorVisible(true); 
        window.setView(uiView);

        window.draw(screenDarkener);

        Sprite panel(panelTex), title(titleTex);

        setupSprite(panel, panelTex, panelPos, panelScale);
        setupSprite(title, titleTex, titlePos, titleScale);

        window.draw(panel);
        window.draw(title);

        Text question(endFont, "Are you sure you want to quit?", 20);
        question.setFillColor(whiteText);
        question.setOutlineThickness(1.f);
        question.setOutlineColor(Color(0, 0, 0, 150));
        centerText(question, WindowWidth / 2.f, 225.f);
        window.draw(question);

        Text hint(endFont, "(Select quit option)", 14);
        hint.setFillColor(whiteText);
        hint.setOutlineThickness(1.f);
        hint.setOutlineColor(Color(0, 0, 0, 150));
        centerText(hint, WindowWidth / 2.f, 250.f);
        window.draw(hint);

        window.draw(saveBtn);
        window.draw(quitBtn);
        window.draw(cancelBtn);

        Text saveTxt(endFont, "Save and Quit", 28);
        saveTxt.setFillColor(saveHover ? yellowHover : greenText);
        saveTxt.setOutlineThickness(2.f);
        saveTxt.setOutlineColor(outlineColor);
        centerText(saveTxt, savePos.x, savePos.y - 2.f);
        window.draw(saveTxt);

        Text quitTxt(endFont, "Quit but don't save", 28);
        quitTxt.setFillColor(quitHover ? yellowHover : greenText);
        quitTxt.setOutlineThickness(2.f);
        quitTxt.setOutlineColor(outlineColor);
        centerText(quitTxt, quitPos.x, quitPos.y - 2.f);
        window.draw(quitTxt);

        Text cancelTxt(endFont, "Don't Quit", 28);
        cancelTxt.setFillColor(cancelHover ? yellowHover : greenText);
        cancelTxt.setOutlineThickness(2.f);
        cancelTxt.setOutlineColor(outlineColor);
        centerText(cancelTxt, cancelPos.x, cancelPos.y - 2.f);
        window.draw(cancelTxt);
        bool endMenuHovering = saveHover || quitHover || cancelHover;
        if (endMenuHovering) { if (playsound) { buttonpressedsound.play(); playsound = 0; } }
        else playsound = 1;
        window.display();
    }

    return false;
}

void ShowPauseMenu()
{
    static bool loaded = false;
    static Texture panelTex, titleTex, fgTex;
    static Texture shortBtnNormalTex, shortBtnHoverTex;
    static Texture longBtnNormalTex, longBtnHoverTex;
    static Font pauseFont;

    if (!loaded)
    {
        if (!pauseFont.openFromFile("Assets/Fonts/BernardMT.ttf"))
            cout << "Missing BernardMT.ttf\n";

        if (!panelTex.loadFromFile("Assets/Pause/pause_menu.png"))
            cout << "Missing pause_menu.png\n";
        if (!titleTex.loadFromFile("Assets/Pause/shell_gamepaused_hdr.png"))
            cout << "Missing shell_gamepaused_hdr.png\n";
        if (!fgTex.loadFromFile("Assets/Pause/shell_popupfg_gamepaused.png"))
            cout << "Missing shell_popupfg_gamepaused.png\n";
        if (!shortBtnNormalTex.loadFromFile("Assets/Pause/shell_largebtn152_normal.png"))
            cout << "Missing shell_largebtn152_normal.png\n";
        if (!shortBtnHoverTex.loadFromFile("Assets/Pause/shell_largebtn152_high.png"))
            cout << "Missing shell_largebtn152_high.png\n";
        if (!longBtnNormalTex.loadFromFile("Assets/Pause/shell_largebtn225_normal.png"))
            cout << "Missing shell_largebtn225_normal.png\n";
        if (!longBtnHoverTex.loadFromFile("Assets/Pause/shell_largebtn225_high.png"))
            cout << "Missing shell_largebtn225_high.png\n";

        pauseFont.setSmooth(true);
        panelTex.setSmooth(true);
        titleTex.setSmooth(true);
        fgTex.setSmooth(true);
        shortBtnNormalTex.setSmooth(true);
        shortBtnHoverTex.setSmooth(true);
        longBtnNormalTex.setSmooth(true);
        longBtnHoverTex.setSmooth(true);

        loaded = true;
    }

    auto centerText = [&](Text &txt, float x, float y)
    {
        FloatRect b = txt.getLocalBounds();
        txt.setOrigin({b.position.x + b.size.x / 2.f, b.position.y + b.size.y / 2.f});
        txt.setPosition({x, y});
    };

    auto setupSprite = [&](Sprite &s, const Texture &tex, Vector2f pos, Vector2f scale)
    {
        s.setTexture(tex, true);
        FloatRect b = s.getLocalBounds();
        s.setOrigin({b.size.x / 2.f, b.size.y / 2.f});
        s.setPosition(pos);
        s.setScale(scale);
    };

    const Color greenText(160, 211, 74);
    const Color yellowHover(255, 241, 74);
    const Color outlineColor(100, 78, 42);

    const Vector2f panelPos = {WindowWidth / 2.f, 318.f};
    const Vector2f panelScale = {1.f, 1.f};

    const Vector2f fgPos = {WindowWidth / 2.f, 408.f};
    const Vector2f fgScale = {1.f, 1.f};

    const Vector2f titlePos = {WindowWidth / 2.f, 140.f};
    const Vector2f titleScale = {1.f, 1.f};

    const Vector2f longScale = {1.f, 1.f};
    const Vector2f shortScale = {1.f, 1.f};

    const Vector2f resumePos = {WindowWidth / 2.f, 235.f};
    const Vector2f optionsPos = {WindowWidth / 2.f, 320.f};
    const Vector2f endPos = {WindowWidth / 2.f, 405.f};

    screenDarkener.setSize(Vector2f(WindowWidth, WindowHeight));
    screenDarkener.setPosition({0.f, 0.f});
    screenDarkener.setFillColor(Color(0, 0, 0, 120));

    Clock frameClock;
    while (window.isOpen() && isLevelRunning)
    {
        float dt = frameClock.restart().asSeconds();

        while (const optional event = window.pollEvent())
        {
            if (event->is<Event::Closed>())
            {
                window.close();
                return;
            }

            if (const auto *resizeEvent = event->getIf<Event::Resized>())
            {
                // view.setViewport(FloatRect({0.f, 0.f}, {1.f, 1.f}));
                // window.setView(view);
            }

            if (const auto *keyPressed = event->getIf<Event::KeyPressed>())
            {
                if (keyPressed->code == Keyboard::Key::Escape)
                    return;
            }

            if (const auto *mouseEvent = event->getIf<Event::MouseButtonReleased>())
            {
                if (mouseEvent->button == Mouse::Button::Left)
                {
                    Vector2f mousePos = window.mapPixelToCoords(Mouse::getPosition(window), uiView);

                    Sprite resumeBtn(longBtnNormalTex), optionsBtn(shortBtnNormalTex), endBtn(longBtnNormalTex);

                    setupSprite(resumeBtn, longBtnNormalTex, resumePos, longScale);
                    setupSprite(optionsBtn, shortBtnNormalTex, optionsPos, shortScale);
                    setupSprite(endBtn, longBtnNormalTex, endPos, longScale);

                    if (resumeBtn.getGlobalBounds().contains(mousePos))
                        return;

                    if (optionsBtn.getGlobalBounds().contains(mousePos))
                    {

                        g_optionsFromPause = true;
                        OptionsMenu();
                        g_optionsFromPause = false;

                        if (!window.isOpen() || !isLevelRunning)
                            return;
                        window.setView(view);
                        frameClock.restart();
                    }

                    if (endBtn.getGlobalBounds().contains(mousePos))
                    {
                        if (ShowEndGameMenu())
                            return;
                    }
                }
            }
        }

        Vector2f mousePos = window.mapPixelToCoords(Mouse::getPosition(window), uiView);

        Sprite resumeBtn(longBtnNormalTex), optionsBtn(shortBtnNormalTex), endBtn(longBtnNormalTex);

        setupSprite(resumeBtn, longBtnNormalTex, resumePos, longScale);
        setupSprite(optionsBtn, shortBtnNormalTex, optionsPos, shortScale);
        setupSprite(endBtn, longBtnNormalTex, endPos, longScale);

        bool resumeHover = resumeBtn.getGlobalBounds().contains(mousePos);
        bool optionsHover = optionsBtn.getGlobalBounds().contains(mousePos);
        bool endHover = endBtn.getGlobalBounds().contains(mousePos);

        static bool wasResumeHover = false, wasOptionsHover = false, wasEndHover = false;
        if ((resumeHover && !wasResumeHover) || (optionsHover && !wasOptionsHover) || (endHover && !wasEndHover))
        {
            if (isSoundEnabled)
                buttonpressedsound.play();
        }
        wasResumeHover = resumeHover;
        wasOptionsHover = optionsHover;
        wasEndHover = endHover;

        if (resumeHover)
            setupSprite(resumeBtn, longBtnHoverTex, resumePos, longScale);
        if (optionsHover)
            setupSprite(optionsBtn, shortBtnHoverTex, optionsPos, shortScale);
        if (endHover)
            setupSprite(endBtn, longBtnHoverTex, endPos, longScale);

        window.clear(Color(20, 100, 160));
        Drawbglevel();                      
        window.setMouseCursorVisible(true);
        window.setView(uiView);

        window.draw(screenDarkener);

        Sprite panel(panelTex), title(titleTex), fg(fgTex);

        setupSprite(panel, panelTex, panelPos, panelScale);
        setupSprite(title, titleTex, titlePos, titleScale);
        setupSprite(fg, fgTex, fgPos, fgScale);

        window.draw(panel);
        fg.setColor(Color(255, 255, 255, 170));
        window.draw(fg);
        window.draw(title);

        window.draw(resumeBtn);
        window.draw(optionsBtn);
        window.draw(endBtn);

        Text resumeTxt(pauseFont, "Resume Game", 36);
        resumeTxt.setFillColor(resumeHover ? yellowHover : greenText);
        resumeTxt.setOutlineThickness(2.f);
        resumeTxt.setOutlineColor(outlineColor);
        centerText(resumeTxt, resumePos.x, resumePos.y - 3.f);
        window.draw(resumeTxt);

        Text optionsTxt(pauseFont, "Options", 36);
        optionsTxt.setFillColor(optionsHover ? yellowHover : greenText);
        optionsTxt.setOutlineThickness(2.f);
        optionsTxt.setOutlineColor(outlineColor);
        centerText(optionsTxt, optionsPos.x, optionsPos.y - 3.f);
        window.draw(optionsTxt);

        Text endTxt(pauseFont, "End Game", 36);
        endTxt.setFillColor(endHover ? yellowHover : greenText);
        endTxt.setOutlineThickness(2.f);
        endTxt.setOutlineColor(outlineColor);
        centerText(endTxt, endPos.x, endPos.y - 3.f);
        window.draw(endTxt);
        bool pauseHovering = resumeHover || optionsHover || endHover;
        if (pauseHovering) { if (playsound) { buttonpressedsound.play(); playsound = 0; } }
        else playsound = 1;

        window.display();
    }
}

void QuitLevelLoadingScreen()
{
    static bool assetsLoaded = false;
    if (!assetsLoaded)
    {
        if (!texLevelLoadingBg.loadFromFile("Assets/QuitLevel/loadbg.png"))
            cout << "Missing level loading bg\n";
        if (!texLevelLoadingLogo.loadFromFile("Assets/QuitLevel/logo.png"))
            cout << "Missing level loading logo\n";
        if (!texLevelLoadBar.loadFromFile("Assets/QuitLevel/loadBar.png"))
            cout << "Missing level load bar\n";
        if (!texLevelLoadBarCap.loadFromFile("Assets/QuitLevel/loadBarCap.png"))
            cout << "Missing level load bar cap\n";
        if (!QuitLevelFont.openFromFile("Assets/Fonts/BarmenoBold.otf"))
            cout << "Missing quit level font\n";

        texLevelLoadingBg.setSmooth(true);
        texLevelLoadingLogo.setSmooth(true);
        texLevelLoadBar.setSmooth(true);
        texLevelLoadBarCap.setSmooth(true);
        mermaidevent.stop();

        float bgScaleX = WindowWidth / texLevelLoadingBg.getSize().x;
        float bgScaleY = WindowHeight / texLevelLoadingBg.getSize().y;
        sprLevelLoadingBg.setTexture(texLevelLoadingBg, true);
        sprLevelLoadingBg.setScale({bgScaleX, bgScaleY});

        float barWidth = 320.f, barHeight = 50.f;
        float barScaleX = barWidth / texLevelLoadBar.getSize().x;
        float barScaleY = barHeight / texLevelLoadBar.getSize().y;

        Vector2u barSize = texLevelLoadBar.getSize();
        int visibleWidth = static_cast<int>(barSize.x * levelLoadingProgress);

        // Logo – centered at top
        sprLevelLoadingLogo.setTexture(texLevelLoadingLogo, true);
        sprLevelLoadingLogo.setOrigin({texLevelLoadingLogo.getSize().x / 2.f,
                                       texLevelLoadingLogo.getSize().y / 2.f});
        sprLevelLoadingLogo.setPosition({WindowWidth / 2.f, 100.f}); 

        // Loading bar cap – centered at bottom
        sprLevelLoadBarCap.setTexture(texLevelLoadBarCap, true);
        sprLevelLoadBarCap.setOrigin({texLevelLoadBarCap.getSize().x / 2.f,
                                      texLevelLoadBarCap.getSize().y / 2.f});
        sprLevelLoadBarCap.setPosition({WindowWidth / 2.f, WindowHeight - 85.f});
        sprLevelLoadBarCap.setScale({barScaleX, barScaleY});

        // Loading bar fill – same origin and position
        sprLevelLoadBar.setTexture(texLevelLoadBar, true);
        sprLevelLoadBar.setOrigin({texLevelLoadBar.getSize().x / 2.f,
                                   texLevelLoadBar.getSize().y / 2.f});
        sprLevelLoadBar.setPosition({WindowWidth / 2.f, WindowHeight - 85.f});
        sprLevelLoadBar.setScale({barScaleX, barScaleY});

        txtQuitLevel.setFillColor(Color(255, 255, 255));
        txtQuitLevel.setOutlineColor(Color(0, 0, 0));
        txtQuitLevel.setOutlineThickness(1.5f);
        txtQuitLevel.setPosition({(WindowWidth / 2.f) - 45.f, 250.f});

        assetsLoaded = true;
    }

    // 2. Animate the bar filling
    Clock totalClock;
    float startTime = 0.5f;    // short pause before filling starts
    float fillDuration = 2.0f; // how many seconds to fill completely

    levelLoadingProgress = 0.f;

    while (window.isOpen())
    {

        float elapsed = totalClock.getElapsedTime().asSeconds();


        while (const optional event = window.pollEvent())
        {
            if (event->is<Event::Closed>())
                window.close();
        }

        // Update progress
        if (elapsed >= startTime)
        {
            levelLoadingProgress = (elapsed - startTime) / fillDuration;
            if (levelLoadingProgress > 1.f)
                levelLoadingProgress = 1.f;
        }

        // Crop the fill bar texture
        Vector2u barSize = texLevelLoadBar.getSize();
        int visibleWidth = static_cast<int>(barSize.x * levelLoadingProgress);
        sprLevelLoadBar.setTextureRect(IntRect({0, 0}, {visibleWidth, (int)barSize.y}));

        // Draw
        window.setView(uiView); 
        window.clear();
        window.draw(sprLevelLoadingBg);
        window.draw(sprLevelLoadingLogo);
        window.draw(sprLevelLoadBarCap);
        window.draw(sprLevelLoadBar);
        window.draw(txtQuitLevel);
        window.display();

        if (levelLoadingProgress >= 1.f)
            break;
    }

    // Small delay so the full bar is visible
    sleep(milliseconds(300));
}

void LevelHud()
{
    StartLevelHud();
    isLevelRunning = true;
    goToMainMenuFromLevel = false;
    isTimeAttackMode = true;

    Clock clock;

    while (window.isOpen() && isLevelRunning)
    {
        float dt = clock.restart().asSeconds();

        if (isTimeAttackMode)
        {
            timeAttackTimer += dt;
            if (timeAttackTimer >= timeAttackLimit)
            {

                isLevelRunning = false;
            }
        }

        UpdateLevelHud();
        DrawLevelHud();
        window.display();
    }

    QuitLevelLoadingScreen();
    goToMainMenuFromLevel = true;
}

// ============================================================
//        END LEVEL - Textures & Sprites
// ============================================================

void StartEndLevel(RenderWindow& window)
{
    score_1 = smallFishEatenCount;
    score_2 = mediumFishEatenCount;
    score_3 = largeFishEatenCount;
    score_4 = score;

    Vector2f winSize(WindowWidth, WindowHeight);
    
    if (texEndBg.getSize().x == 0 || texEndBg.getSize().y == 0)
    {
        std::cerr << "Error: Failed to load End Level Background!" << std::endl;
    }
    else
    {
        Vector2f texSize = Vector2f(texEndBg.getSize());
        sprEndBg.setScale({ winSize.x / texSize.x, winSize.y / texSize.y });
    }

    score_of_eaten_fish[0].setString(to_string(score_1));
    score_of_eaten_fish[1].setString(to_string(score_2));
    score_of_eaten_fish[2].setString(to_string(score_3));
    score_of_eaten_fish[3].setString(to_string(starsEatenCount));

    string formattedTotal = "";
    int temp = score_4;
    int cnt = 0;
    while (temp > 0) {
        if (cnt == 3) formattedTotal = "," + formattedTotal;
        formattedTotal = to_string(temp % 10) + formattedTotal;
        temp /= 10;
        cnt++;
    }
    if (formattedTotal.empty()) formattedTotal = "0";
    score_of_eaten_fish[4].setString(formattedTotal);

    for (int i = 0; i < 5; i++) {
        score_of_eaten_fish[i].setFont(quit_option_font);
        score_of_eaten_fish[i].setFillColor(Color::White);
        score_of_eaten_fish[i].setOutlineColor(Color::Black);
        score_of_eaten_fish[i].setOutlineThickness(2);
        FloatRect bounds = score_of_eaten_fish[i].getLocalBounds();
        score_of_eaten_fish[i].setOrigin({ bounds.size.x / 2.f, bounds.size.y / 2.f });
    }

    score_of_eaten_fish[0].setCharacterSize(20);
    score_of_eaten_fish[1].setCharacterSize(20);
    score_of_eaten_fish[2].setCharacterSize(20);
    score_of_eaten_fish[3].setCharacterSize(20);
    score_of_eaten_fish[4].setCharacterSize(26);

    struct BtnInfo { const char* n; const char* h; float x, y, sc, hsc, hw, hh; };
    BtnInfo btnInfos[3] = {
        {"Assets/GameScreen/option1.png", "Assets/GameScreen/option2.png", 180.f, 560.f, 1/10.f, 1/10.f, 0.f, 0.f},
        {"Assets/GameScreen/quit1.png", "Assets/GameScreen/quit2.png", 630.f, 560.f, 2/10.f, 2/10.f, 0.f, 0.f},
        {"Assets/EndGameScreen/continue_normal-1 (1).png", "Assets/EndGameScreen/continue_high-1 (1).png", 400.f, 540.f, 5.f/10.f, 5.f/10.f, 80.f, 30.f},
    };

    for (int i = 0; i < 3; i++)
    {
        if (!gsBtns[i].normalTex.loadFromFile(btnInfos[i].n)) cout << "Error btn normal " << i << ": " << btnInfos[i].n << "\n";
        if (!gsBtns[i].hoverTex.loadFromFile(btnInfos[i].h))  cout << "Error btn hover "  << i << ": " << btnInfos[i].h << "\n";
        
        gsBtns[i].normalTex.setSmooth(true);
        gsBtns[i].hoverTex.setSmooth(true);
        gsBtns[i].x          = btnInfos[i].x;
        gsBtns[i].y          = btnInfos[i].y;
        gsBtns[i].scale      = btnInfos[i].sc;
        gsBtns[i].hoverScale = btnInfos[i].hsc;
        gsBtns[i].hoverHalfW = btnInfos[i].hw;
        gsBtns[i].hoverHalfH = btnInfos[i].hh;
        
        gsBtns[i].sprite = sf::Sprite(gsBtns[i].normalTex);
        gsBtns[i].sprite->setOrigin({ gsBtns[i].normalTex.getSize().x / 2.f, gsBtns[i].normalTex.getSize().y / 2.f });
        gsBtns[i].sprite->setPosition({ gsBtns[i].x, gsBtns[i].y });
        gsBtns[i].sprite->setScale({ gsBtns[i].scale, gsBtns[i].scale });
    }
}
bool EndLevel()
{
    g_inEndScreen = true; 
    mermaidevent.stop();
    view.setSize({ 800.f, 600.f });
    view.setCenter({ 400.f, 300.f });
    window.setView(view);
    window.setMouseCursorVisible(true);

    StartEndLevel(window);

    bool endScreenRunning = true;

    while (window.isOpen() && endScreenRunning)
    {
        while (const optional event = window.pollEvent())
        {
            if (event->is<Event::Closed>()) 
            {
                window.close();
                return false;
            }

            else if (const auto *keyPressed = event->getIf<Event::KeyPressed>())
            {
                if (keyPressed->code == Keyboard::Key::Escape)
                {
                    if (showQuitPopup)
                    {
                        showQuitPopup = false;
                    }
                    }
                }

            if (const auto* mouseBtn = event->getIf<Event::MouseButtonReleased>())
            {
                if (mouseBtn->button == Mouse::Button::Left)
                {
                    Vector2f mousePos = window.mapPixelToCoords(Mouse::getPosition(window));

                    if (gsBtns[0].sprite && gsBtns[0].sprite->getGlobalBounds().contains(mousePos))
                    {
                    g_optionsFromPause = true; 
                    OptionsMenu();
                    g_optionsFromPause = false; 
                    }

                    if (gsBtns[1].sprite && gsBtns[1].sprite->getGlobalBounds().contains(mousePos))
                    {
                        showQuitPopup = true;
                    }

                    if (gsBtns[2].sprite)
                    {
                        bool hovered = mousePos.x >= gsBtns[2].x - gsBtns[2].hoverHalfW &&
                                       mousePos.x <= gsBtns[2].x + gsBtns[2].hoverHalfW &&
                                       mousePos.y >= gsBtns[2].y - gsBtns[2].hoverHalfH &&
                                       mousePos.y <= gsBtns[2].y + gsBtns[2].hoverHalfH;
                        
                    if (hovered)
                    {
                        levelWonSuccessfully = true;
                        SaveGameData();
                        
                        if (levelsound.getStatus() == Music::Status::Playing)
                            levelsound.stop();
                        if (WaveSound.getStatus() == Music::Status::Playing)
                            WaveSound.stop();

                        if (mermaidevent.getStatus() == SoundSource::Status::Playing)
                            mermaidevent.stop();

                        mainmenumusic.play();
                        mainmenumusic.setLooping(true);
                        
                        endScreenRunning = false; 
                    }
                    }

                    if (showQuitPopup)
                    {
                        if (quitPopup.btns[0].sprite && quitPopup.btns[0].sprite->getGlobalBounds().contains(mousePos))
                        {
                            showQuitPopup = false;
                            levelsound.stop();
                            WaveSound.stop();
                            mermaidevent.stop();
                            // QuitLevelLoadingScreen();
                            isLevelRunning = false;
                            goToMainMenuFromLevel = true;
                            return true; 
                        }
                        if (quitPopup.btns[1].sprite && quitPopup.btns[1].sprite->getGlobalBounds().contains(mousePos))
                        {
                            showQuitPopup = false;
                            levelsound.stop();
                            WaveSound.stop();
                            mermaidevent.stop();
                            // QuitLevelLoadingScreen();
                            isLevelRunning = false;
                            goToMainMenuFromLevel = true;
                            return true;
                        }

                        if (quitPopup.btns[2].sprite && quitPopup.btns[2].sprite->getGlobalBounds().contains(mousePos))
                        {
                            showQuitPopup = false;
                        }
                    }
                }
            }
        }

        window.setView(view);
        UpdateEndLevel(window);
        DrawEndLevel(window);
    }
    g_inEndScreen = false; 
    return true;
}

void UpdateEndLevel(RenderWindow& window)
{
    Vector2f mousePos = window.mapPixelToCoords(Mouse::getPosition(window));

    for (int i = 0; i < 2; i++) 
    {
        if (!gsBtns[i].sprite) continue;
        if (gsBtns[i].sprite->getGlobalBounds().contains(mousePos))
        {
            gsBtns[i].sprite->setTexture(gsBtns[i].hoverTex);
            gsBtns[i].sprite->setOrigin({ gsBtns[i].hoverTex.getSize().x / 2.f,
                                           gsBtns[i].hoverTex.getSize().y / 2.f });
        }
        else
        {
            gsBtns[i].sprite->setTexture(gsBtns[i].normalTex);
            gsBtns[i].sprite->setOrigin({ gsBtns[i].normalTex.getSize().x / 2.f,
                                           gsBtns[i].normalTex.getSize().y / 2.f });
        }
    }

    if (gsBtns[2].sprite)
    {
        bool hovered = mousePos.x >= gsBtns[2].x - gsBtns[2].hoverHalfW &&
                       mousePos.x <= gsBtns[2].x + gsBtns[2].hoverHalfW &&
                       mousePos.y >= gsBtns[2].y - gsBtns[2].hoverHalfH &&
                       mousePos.y <= gsBtns[2].y + gsBtns[2].hoverHalfH;
        if (hovered)
        {
            gsBtns[2].sprite->setTexture(gsBtns[2].hoverTex);
            gsBtns[2].sprite->setOrigin({ gsBtns[2].hoverTex.getSize().x / 2.f,
                                           gsBtns[2].hoverTex.getSize().y / 2.f });
            gsBtns[2].sprite->setScale({ gsBtns[2].hoverScale, gsBtns[2].hoverScale });
        }
        else
        {
            gsBtns[2].sprite->setTexture(gsBtns[2].normalTex);
            gsBtns[2].sprite->setOrigin({ gsBtns[2].normalTex.getSize().x / 2.f,
                                           gsBtns[2].normalTex.getSize().y / 2.f });
            gsBtns[2].sprite->setScale({ gsBtns[2].scale, gsBtns[2].scale });
        }
    }

    if (showQuitPopup)
    {
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
    bool endHovering = false;
    for (int i = 0; i < 3; i++)
        if (gsBtns[i].sprite && gsBtns[i].sprite->getGlobalBounds().contains(mousePos))
            endHovering = true;
    if (showQuitPopup)
        for (int i = 0; i < 3; i++)
            if (quitPopup.btns[i].sprite && quitPopup.btns[i].sprite->getGlobalBounds().contains(mousePos))
                endHovering = true;
    if (endHovering) { if (playsound) { buttonpressedsound.play(); playsound = 0; } }
    else playsound = 1;
}

void DrawEndLevel(RenderWindow& window)
{
    window.clear();
    window.setView(view);
    
    window.draw(sprEndBg);

    Vector2f winSize(WindowWidth, WindowHeight);

    {
        string content = "STAGE COMPLETE!";
        Text shadow(st_comp, content, 48);
        shadow.setStyle(Text::Bold | Text::Italic);
        shadow.setFillColor(Color::Black);
        
        Text main(st_comp, content, 48);
        main.setStyle(Text::Bold | Text::Italic);
        main.setFillColor(Color(255, 215, 0)); 
        main.setOutlineColor(Color(50, 50, 0));
        main.setOutlineThickness(2);

        FloatRect b = main.getLocalBounds();
        main.setOrigin({ b.size.x / 2.f, b.size.y / 2.f });
        shadow.setOrigin({ b.size.x / 2.f, b.size.y / 2.f });
        
        Vector2f pos = { 400.f, 65.f };
        shadow.setPosition({ pos.x + 3.f, pos.y + 3.f });
        main.setPosition(pos);
        window.draw(shadow); window.draw(main);
    }

    {
        string content = "Lunch Report";
        Text main(quit_option_font, content, 22);
        main.setStyle(Text::Underlined);
        main.setFillColor(Color::White);
        main.setOutlineColor(Color::Black);
        main.setOutlineThickness(2);
        FloatRect b = main.getLocalBounds();
        main.setOrigin({ b.size.x / 2.f, b.size.y / 2.f });
        main.setPosition({ 400.f, 135.f });
        window.draw(main);
    }

    {
        string content = "Food Bank Bonus:";
        Text shadow(quit_option_font, content, 18);
        shadow.setFillColor(Color::Black);
        Text main(quit_option_font, content, 18);
        main.setFillColor(Color(200, 255, 200)); 
        main.setOutlineColor(Color::Black);
        main.setOutlineThickness(1);
        FloatRect b = main.getLocalBounds();
        main.setOrigin({ b.size.x / 2.f, b.size.y / 2.f });
        shadow.setOrigin({ b.size.x / 2.f, b.size.y / 2.f });
        Vector2f pos = { 250.f, 165.f }; 
        shadow.setPosition({ pos.x + 1.f, pos.y + 1.f });
        main.setPosition(pos);
        window.draw(shadow); window.draw(main);
    }

    {
        string content = "Food Bank:";
        Text shadow(quit_option_font, content, 18);
        shadow.setFillColor(Color::Black);
        Text main(quit_option_font, content, 18);
        main.setFillColor(Color::White);
        main.setOutlineColor(Color::Black);
        main.setOutlineThickness(1);
        FloatRect b = main.getLocalBounds();
        main.setOrigin({ b.size.x / 2.f, b.size.y / 2.f });
        shadow.setOrigin({ b.size.x / 2.f, b.size.y / 2.f });
        Vector2f pos = { 250.f, 190.f };
        shadow.setPosition({ pos.x + 1.f, pos.y + 1.f });
        main.setPosition(pos);
        window.draw(shadow); window.draw(main);
        
        score_of_eaten_fish[4].setPosition({ 400.f, 170.f }); 
        window.draw(score_of_eaten_fish[4]);
    }

    float fishY = 220.f;
    float fishSpacing = 90.f;
    float startX = 280.f; 

    sprHerring.setPosition({startX - 32.f, fishY - 20.f});
    window.draw(sprHerring);
    sprCod.setPosition({startX - 29.f + fishSpacing, fishY - 20.f});
    window.draw(sprCod);
    sprLionfish.setPosition({startX - 25.f + fishSpacing*2, fishY - 20.f});
    window.draw(sprLionfish);
    sprStarBubble.setPosition({startX - 30.f + fishSpacing*3, fishY - 35.f});
    window.draw(sprStarBubble);

    static bool timerInitialized = false;
    static Clock timer;
    if (!timerInitialized) {
        timer.restart();
        timerInitialized = true;
    }

    float elapsed = timer.getElapsedTime().asSeconds();
    float stageDuration = 0.3f; 
    
    for (int i = 0; i <4; i++) {
        float startTime = i * stageDuration;
        float endTime   = startTime + (stageDuration / 2.f);
        
        if (elapsed >= startTime) {
            if (elapsed < endTime) {
                sprCrunch.setPosition({ startX + (i * fishSpacing), fishY + 25.f });
                window.draw(sprCrunch);
            } else {
                score_of_eaten_fish[i].setPosition({ startX + (i * fishSpacing), fishY + 35.f });
                window.draw(score_of_eaten_fish[i]);
            }
        }
    }

    {
        string content = "You've earned rank of:";
        Text main(quit_option_font, content, 18);
        main.setFillColor(Color::White);
        main.setOutlineColor(Color::Black);
        main.setOutlineThickness(1);
        FloatRect b = main.getLocalBounds();
        main.setOrigin({ b.size.x / 2.f, b.size.y / 2.f });
        main.setPosition({ 400.f, 310.f });
        window.draw(main);

        Text rank(quit_option_font);
        int total = score_1 + score_2 + score_3;
        if (total < 100) rank.setString("Guppie Guzzler");
        else rank.setString("Barracuda Bully"); 
        
        rank.setCharacterSize(24);
        rank.setFillColor(Color(100,255,100)); 
        rank.setOutlineColor(Color::Black);
        rank.setOutlineThickness(2);
        FloatRect rb = rank.getLocalBounds();
        rank.setOrigin({ rb.size.x / 2.f, rb.size.y / 2.f });
        rank.setPosition({ 400.f, 345.f });
        window.draw(rank);
    }

    {
        string content = "Fun Fact";
        Text shadow(quit_option_font, content, 22);
        shadow.setFillColor(Color::Black);
        Text main(quit_option_font, content, 22);
        main.setStyle(Text::Underlined);
        main.setFillColor(Color(255, 200, 100)); 
        main.setOutlineColor(Color::Black);
        main.setOutlineThickness(2);
        FloatRect b = main.getLocalBounds();
        main.setOrigin({ b.size.x / 2.f, b.size.y / 2.f });
        shadow.setOrigin({ b.size.x / 2.f, b.size.y / 2.f });
        Vector2f pos = { 400.f, 385.f };
        shadow.setPosition({ pos.x + 2.f, pos.y + 2.f });
        main.setPosition(pos);
        window.draw(shadow); window.draw(main);
    }

    {
        string content = "Herrings communicate with each other by producing high-""\n frequency sounds from their bottoms ""\n (basically, they communicate through farts!) It""\n helps them stay together in schools at night";
        Text main(quit_option_font, content, 16);
        main.setFillColor(Color::White);
        main.setOutlineColor(Color::Black);
        main.setOutlineThickness(1);
        FloatRect b = main.getLocalBounds();
        main.setOrigin({ b.size.x / 2.f, b.size.y / 2.f });
        main.setPosition({ 420.f, 450.f });
        window.draw(main);
    }

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

    window.display();
}