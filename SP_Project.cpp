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
using namespace std; // عشان نكتب أوامر C++ العادية بسهولة
using namespace sf;  // عشان نستخدم أوامر SFML من غير تعقيد

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
void CreateButton(Sprite &sprite, Texture &texture, const string &filePath, // A Template for creating sprites
                  float xPosition,
                  float yPositon, float scaleX, float scaleY);
void SetupButtonText(Text &text, const string &str, Sprite &button); // A Template for Creating Text for buttons
void UpdateSwitchUser();
void HoverButton(Sprite &button, const Texture &normalTex,
                 const Texture &highlightTex, Vector2f mousePos, Text &text); // A Template for Hovering Over Buttons
void EnterYourName();
void RefreshUsersList();
void DeleteUser();
void FullList();       // If the List is Full -> (max is 7 users)
void DupplicateUser(); // If the user tried to enter a name that already exists
void DisplaySwitchUser();
void Select_level();
void StartSelectLevel();
void UpdateSelectLevel(float dt = 0.f);
void DrawSelectLevel();
// [ADDED] New function declaration for Options menu (placed with all other function declarations)
void OptionsMenu();
void StartOptions();
void UpdateOptions();
void DrawOptions();
// [ADDED] Forward declarations for background fish updates
void UpdateMainMenuFish();
void DrawMainMenuBackground();
// sound effects for each button
void PlayingSound(bool isMainMenu);
// function quit
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
// --- المتغيرات العالمية ---
float WindowWidth = 800;                                                                                     // عرض الشاشة الثابت
float WindowHeight = 600;                                                                                    // طول الشاشة الثابت
RenderWindow window(VideoMode({(unsigned int)WindowWidth, (unsigned int)WindowHeight}), "Feeding Frenzy 2"); // فتح الشاشة كاملة
View view({400.f, 300.f}, Vector2f(window.getSize()));                                                       // الكاميرا اللي بتظبط أبعاد اللعبة
// =================================== Loading Screen =================================
Shader swayShader;                                                                                                       // "فلتر" حركة المية الهادية
float deltaTime;                                                                                                         // الوقت بين كل فريم والتاني
float startX = WindowWidth / 2.f;                                                                                        // نقطة بداية رص الحروف في النص
float spacing = 65.0f;                                                                                                   // المسافة الفاضية بين كل حرف وتاني
float width_bubble_global;                                                                                               // عرض الفقاعة الكبيرة اللي ورا الحرف
float endTimer = 0.0f;                                                                                                   // تايمر بيبدأ بعد ما الحروف تفرقع
bool transitionStarted = false;                                                                                          // علامة إننا بدأنا وقت الانتظار الأخير
Vector2f ventPositions[2] = {{WindowWidth - 140.f, WindowHeight - 157.5f}, {WindowWidth - 220.f, WindowHeight - 117.f}}; // أماكن فتحات المرجان
struct Particle
{                         // وصف لأي ذرة (شرارة أو فقاعة فرقعة)
    float x, y, vx, vy;   // مكانها وسرعتها في الاتجاهين
    float alpha = 255.0f; // درجة وضوحها (بتختفي بالتدريج)
    bool active = false;  // هل هي ظاهرة دلوقتي ولا لأ؟
};
struct AmbientBubble
{                         // وصف لفقاقيع الجو العام
    float x, y, vx, vy;   // مكانها وسرعتها
    float alpha = 255.0f; // شفافيتها
    bool active = false;  // هل هي شغالة؟
    bool isVent = false;  // هل طالعة من المرجان ولا بالعرض؟
};
AmbientBubble ventBubbles[60];  // مصفوفة فيها 60 فقاعة للجو
float bubbleTimer = 0.0f;       // تايمر عشان نطلع فقاقيع بالعرض
Particle sparks[35];            // 35 شرارة للانفجار
Particle letterBubbles[35];     // 35 فقاعة صغيرة للانفجار
bool exploded = false;          // هل الفرقعة حصلت خلاص؟
bool particlesFinished = false; // هل كل آثار الفرقعة اختفت؟
float timerLeft = 0.0f;         // تايمر للمرجان اللي على الشمال
float timerRight = 0.0f;        // تايمر للمرجان اللي على اليمين

// ==========================================
// ===== GameScreen =====
// ==========================================
float WindowWidth = 800;
float WindowHeight = 600;
// --- خلفية الـ GameScreen ---
Texture gameScreenBgTexture;                        // تكسچر خلفية شاشة اللعبة
Sprite gameScreenBgSprite(gameScreenBgTexture);     // سبرايت خلفية شاشة اللعبة
bool gameScreenActive = false;                      // هل شاشة اللعبة شغالة؟
int selectedLevel = -1;                             // رقم اللفل اللي اتاختار (-1 = مفيش)

// --- زرار Options (btn1) ---
Texture gsBtn1NormalTex, gsBtn1HoverTex;            // تكسچر زرار Options عادي و hover
optional<Sprite> gsBtn1Sprite;                      // سبرايت زرار Options
const float GS_BTN1_X = 180.f;                     // مكان زرار Options على محور X
const float GS_BTN1_Y = 560.f;                     // مكان زرار Options على محور Y
const float GS_BTN1_SCALE = 1 / 10.0f;             // حجم زرار Options

// --- زرار Quit (btn2) ---
Texture gsBtn2NormalTex, gsBtn2HoverTex;            // تكسچر زرار Quit عادي و hover
optional<Sprite> gsBtn2Sprite;                      // سبرايت زرار Quit
const float GS_BTN2_X = 630.f;                     // مكان زرار Quit على محور X
const float GS_BTN2_Y = 560.f;                     // مكان زرار Quit على محور Y
const float GS_BTN2_SCALE = 2 / 10.0f;             // حجم زرار Quit

// --- زرار Continue (btn3) ---
Texture gsBtn3NormalTex, gsBtn3HoverTex;            // تكسچر زرار Continue عادي و hover
optional<Sprite> gsBtn3Sprite;                      // سبرايت زرار Continue
const float GS_BTN3_X = 400.f;                     // مكان زرار Continue على محور X
const float GS_BTN3_Y = 540.f;                     // مكان زرار Continue على محور Y
const float GS_BTN3_SCALE = 1.5 / 10.0f;           // حجم زرار Continue عادي
const float GS_BTN3_HOVER_SCALE = 3.5 / 10.0f;     // حجم زرار Continue لما الماوس فوقه
const float GS_BTN3_HOVER_HALF_W = 80.f;           // نص عرض منطقة الكليك على Continue
const float GS_BTN3_HOVER_HALF_H = 30.f;           // نص ارتفاع منطقة الكليك على Continue

// --- Quit Popup ---
bool showQuitPopup = false;                         // هل نافذة تأكيد الخروج ظاهرة؟
Texture quitPopupBgTex;                             // تكسچر خلفية نافذة الخروج
optional<Sprite> quitPopupBgSprite;                 // سبرايت خلفية نافذة الخروج
Texture quitPopupTitleTex;                          // تكسچر عنوان نافذة الخروج
optional<Sprite> quitPopupTitleSprite;              // سبرايت عنوان نافذة الخروج
Texture quitPopupBtn1NormalTex, quitPopupBtn1HoverTex; // تكسچر زرار 1 (Save & Quit)
Texture quitPopupBtn2NormalTex, quitPopupBtn2HoverTex; // تكسچر زرار 2 (Quit without Save)
Texture quitPopupBtn3NormalTex, quitPopupBtn3HoverTex; // تكسچر زرار 3 (Don't Quit)
optional<Sprite> quitPopupBtn1Sprite;               // سبرايت زرار 1 في نافذة الخروج
optional<Sprite> quitPopupBtn2Sprite;               // سبرايت زرار 2 في نافذة الخروج
optional<Sprite> quitPopupBtn3Sprite;               // سبرايت زرار 3 في نافذة الخروج
Text quitPopupLine1(font, "", 22);
Text quitPopupLine2(font, "", 18);
const float QUIT_POPUP_X = 400.f;                  // مكان نافذة الخروج على محور X
const float QUIT_POPUP_Y = 330.f;                  // مكان نافذة الخروج على محور Y
const float QUIT_BTN1_Y = 300.f;                   // مكان زرار 1 على محور Y
const float QUIT_BTN2_Y = 360.f;                   // مكان زرار 2 على محور Y
const float QUIT_BTN3_Y = 420.f;                    // مكان زرار 3 على محور Y
const float QUIT_BTN_SCALE = 0.7f;                 // حجم أزرار نافذة الخروج
const float QUIT_BTN_HOVER_SCALE = 0.6f;           // حجم الأزرار لما الماوس فوقها
const float QUIT_BTN1_HOVER_SCALE = 0.01f;         // حجم hover زرار 1
const float QUIT_BTN2_HOVER_SCALE = 0.001f;        // حجم hover زرار 2
const float QUIT_BTN3_HOVER_SCALE = 0.001f;        // حجم hover زرار 3

// --- Level 1 صور ونصوص ---
Texture lvl1Img1Tex, lvl1Img2Tex, lvl1Img3Tex, lvl1Img4Tex; // تكسچرات صور Level 1
optional<Sprite> lvl1Img1Sprite, lvl1Img2Sprite, lvl1Img3Sprite, lvl1Img4Sprite; // سبرايتات صور Level 1
const float LVL1_IMG1_X = 580.f, LVL1_IMG1_Y = 230.f, LVL1_IMG1_SCALE = 0.4f;  // مكان وحجم صورة 1
const float LVL1_IMG2_X = 300.f, LVL1_IMG2_Y = 403.f, LVL1_IMG2_SCALE = 0.9f;  // مكان وحجم صورة 2
const float LVL1_IMG3_X = 410.f, LVL1_IMG3_Y = 405.f, LVL1_IMG3_SCALE = 0.2f;  // مكان وحجم صورة 3
const float LVL1_IMG4_X = 480.f, LVL1_IMG4_Y = 415.f, LVL1_IMG4_SCALE = 0.6f;  // مكان وحجم صورة 4
const float LVL1_TITLE_X = 415.f, LVL1_TITLE_Y = 55.f;    const int LVL1_TITLE_SIZE = 26;    // موضع وحجم عنوان Level 1
const float LVL1_SUBTITLE_X = 415.f, LVL1_SUBTITLE_Y = 88.f; const int LVL1_SUBTITLE_SIZE = 16; // موضع وحجم subtitle Level 1
const float LVL1_DESC_X = 150.f, LVL1_DESC_Y = 145.f;    const int LVL1_DESC_SIZE = 14;     // موضع وحجم نص الوصف
const float LVL1_HTP_X = 145.f, LVL1_HTP_Y = 250.f;     const int LVL1_HTP_SIZE = 17;      // موضع وحجم عنوان How to Play
const float LVL1_TIPS_X = 160.f, LVL1_TIPS_Y = 280.f;    const int LVL1_TIPS_SIZE = 13;     // موضع وحجم نص التعليمات
const float LVL1_FOOTER_X = 390.f, LVL1_FOOTER_Y = 480.f;  const int LVL1_FOOTER_SIZE = 15;   // موضع وحجم النص السفلي
optional<Text> lvl1TitleText, lvl1SubtitleText, lvl1DescText; // نصوص Level 1 الجزء الأول
optional<Text> lvl1HTPText, lvl1TipsText, lvl1FooterText;     // نصوص Level 1 الجزء التاني
Font lvl1Font;                                                 // خط Level 1

// --- Level 2 صور ونصوص ---
Texture lvl2Img1Tex, lvl2Img2Tex, lvl2Img3Tex, lvl2Img4Tex; // تكسچرات صور Level 2
optional<Sprite> lvl2Img1Sprite, lvl2Img2Sprite, lvl2Img3Sprite, lvl2Img4Sprite; // سبرايتات صور Level 2
const float LVL2_IMG1_X = 580.f, LVL2_IMG1_Y = 200.f, LVL2_IMG1_SCALE = 0.9f;  // مكان وحجم صورة 1
const float LVL2_IMG2_X = 580.f, LVL2_IMG2_Y = 240.f, LVL2_IMG2_SCALE = 0.9f;  // مكان وحجم صورة 2
const float LVL2_IMG3_X = 320.f, LVL2_IMG3_Y = 405.f, LVL2_IMG3_SCALE = 0.9f;  // مكان وحجم صورة 3
const float LVL2_IMG4_X = 440.f, LVL2_IMG4_Y = 415.f, LVL2_IMG4_SCALE = 0.6f;  // مكان وحجم صورة 4
const float LVL2_TITLE_X = 415.f, LVL2_TITLE_Y = 55.f;    const int LVL2_TITLE_SIZE = 26;    // موضع وحجم عنوان Level 2
const float LVL2_SUBTITLE_X = 415.f, LVL2_SUBTITLE_Y = 88.f; const int LVL2_SUBTITLE_SIZE = 16; // موضع وحجم subtitle Level 2
const float LVL2_HEAD1_X = 150.f, LVL2_HEAD1_Y = 145.f;   const int LVL2_HEAD1_SIZE = 17;    // موضع وحجم عنوان Watch for Starfish
const float LVL2_DESC_X = 150.f, LVL2_DESC_Y = 175.f;    const int LVL2_DESC_SIZE = 14;     // موضع وحجم نص الوصف
const float LVL2_HEAD2_X = 150.f, LVL2_HEAD2_Y = 270.f;   const int LVL2_HEAD2_SIZE = 17;    // موضع وحجم عنوان How to Dash
const float LVL2_DASH_X = 150.f, LVL2_DASH_Y = 300.f;    const int LVL2_DASH_SIZE = 14;     // موضع وحجم نص الـ Dash
const float LVL2_FOOTER_X = 390.f, LVL2_FOOTER_Y = 480.f;  const int LVL2_FOOTER_SIZE = 15;   // موضع وحجم النص السفلي
optional<Text> lvl2TitleText, lvl2SubtitleText, lvl2Head1Text; // نصوص Level 2 الجزء الأول
optional<Text> lvl2DescText, lvl2Head2Text, lvl2DashText, lvl2FooterText; // نصوص Level 2 الجزء التاني
Font lvl2Font;                                                             // خط Level 2

// --- Level 3 صور ونصوص ---
Texture lvl3Img1Tex, lvl3Img2Tex, lvl3Img3Tex;               // تكسچرات صور Level 3
optional<Sprite> lvl3Img1Sprite, lvl3Img2Sprite, lvl3Img3Sprite; // سبرايتات صور Level 3
const float LVL3_IMG1_X = 250.f, LVL3_IMG1_Y = 350.f, LVL3_IMG1_SCALE = 1.0f;  // مكان وحجم صورة 1
const float LVL3_IMG2_X = 250.f, LVL3_IMG2_Y = 400.f, LVL3_IMG2_SCALE = 0.8f;  // مكان وحجم صورة 2
const float LVL3_IMG3_X = 400.f, LVL3_IMG3_Y = 280.f, LVL3_IMG3_SCALE = 0.8f;  // مكان وحجم صورة 3
const float LVL3_TITLE_X = 415.f, LVL3_TITLE_Y = 55.f;    const int LVL3_TITLE_SIZE = 26;    // موضع وحجم عنوان Level 3
const float LVL3_SUBTITLE_X = 415.f, LVL3_SUBTITLE_Y = 88.f; const int LVL3_SUBTITLE_SIZE = 16; // موضع وحجم subtitle Level 3
const float LVL3_HEAD1_X = 150.f, LVL3_HEAD1_Y = 145.f;   const int LVL3_HEAD1_SIZE = 17;    // موضع وحجم عنوان Frenzy Time
const float LVL3_DESC_X = 150.f, LVL3_DESC_Y = 175.f;    const int LVL3_DESC_SIZE = 14;     // موضع وحجم نص الوصف
const float LVL3_HEAD2_X = 320.f, LVL3_HEAD2_Y = 320.f;   const int LVL3_HEAD2_SIZE = 17;    // موضع وحجم عنوان Frenzy Boost
const float LVL3_BOOST_X = 320.f, LVL3_BOOST_Y = 345.f;   const int LVL3_BOOST_SIZE = 14;    // موضع وحجم نص الـ Boost
const float LVL3_FOOTER_X = 390.f, LVL3_FOOTER_Y = 450.f;  const int LVL3_FOOTER_SIZE = 15;   // موضع وحجم النص السفلي
optional<Text> lvl3TitleText, lvl3SubtitleText, lvl3Head1Text;             // نصوص Level 3 الجزء الأول
optional<Text> lvl3DescText, lvl3Head2Text, lvl3BoostText, lvl3FooterText; // نصوص Level 3 الجزء التاني
Font lvl3Font;                                                             // خط Level 3


// --- تحميل الصور والأجسام ---
Texture Background_Loading("Assets/Main menu & Loading/Loading Screen/Images/loadbg.png");     // صورة الخلفية
Sprite Background_Loading_sprite(Background_Loading);                                          // جسم الخلفية
Texture Game_Icon("Assets/Main menu & Loading/Loading Screen/Images/LOGO.png");                // لوجو اللعبة
Sprite Game_Icon_sprite(Game_Icon);                                                            // جسم اللوجو
Texture POPCAP_Logo("Assets/Main menu & Loading/Loading Screen/Images/popCapLogo.png");        // لوجو الشركة
Sprite POPCAP_Logo_sprite(POPCAP_Logo);                                                        // جسم لوجو الشركة
Texture LogoWood("Assets/Main menu & Loading/Loading Screen/Images/sproutLogoWood.png");       // الخشبة اللي تحت
Sprite LogoWood_sprite(LogoWood);                                                              // جسم الخشبة
Texture loadBar("Assets/Main menu & Loading/Loading Screen/Images/loadBar.png");               // الشريط المليان
Sprite loadBar_sprite(loadBar);                                                                // جسم الشريط المليان
Texture loadBar_empty("Assets/Main menu & Loading/Loading Screen/Images/loadBarCap.png");      // برواز الشريط
Sprite loadBar_empty_sprite(loadBar_empty);                                                    // جسم البرواز
Texture bubbleTexture("Assets/Main menu & Loading/Loading Screen/Images/bubbles_letters.png"); // فقاعة الحرف
Sprite bubbleSprite(bubbleTexture);                                                            // جسم فقاعة الحرف
Texture fontTexture("Assets/Fonts/LOADING.png");                                               // صورة الحروف المجمعة
Sprite s_L(fontTexture);                                                                       // جسم حرف L
Sprite s_O(fontTexture);                                                                       // جسم حرف O
Sprite s_A(fontTexture);                                                                       // جسم حرف A
Sprite s_D(fontTexture);                                                                       // جسم حرف D
Sprite s_I(fontTexture);                                                                       // جسم حرف I
Sprite s_N(fontTexture);                                                                       // جسم حرف N
Sprite s_G(fontTexture);                                                                       // جسم حرف G
Texture bubbleSmallTex("Assets/Main menu & Loading/Loading Screen/Images/fx_bubble_d0.png");   // فقاعة صغيرة
Sprite bubbleSmallSprite(bubbleSmallTex);                                                      // جسم الفقاعة الصغيرة
Texture spark("Assets/Main menu & Loading/Loading Screen/Images/sparkleb0.png");               // صورة الشرارة
Sprite sparkSprite(spark);                                                                     // جسم الشرارة
// ===============================================================================
// =================================== Main Menu =================================
// ===============================================================================
Texture mainbackground("Assets/Main menu & Loading/Main Menu/mm_mainmenubg.jpg");
Sprite background(mainbackground);
// LOGO TEXTURE
Texture logo("Assets/Main menu & Loading/Main Menu/logo-removebg-preview.png");
Sprite logosp(logo);
// GreenFish Textures & Sprite
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
//_______________________________________________________________________________________________
// Barracuda FISH Textures & Sprites
Texture Barracudatex("Assets/Fish/Barracuda/Barracuda.png");
Sprite Barracuda(Barracudatex);
int BFcol = 0;
int BFrow = 0;
float BFvelocityX_AXIS = -2;
float BFvelocityY_AXIS = -2;
float BFchangedir = -1;
//____________________________________________________
// start game button
Texture startgametex("Assets/Main menu & Loading/Main Menu/mm_startgame_normal-removebg-preview.png");
Texture startgamepressed("Assets/Main menu & Loading/Main Menu/mm_startgame_high-removebg-preview.png");
Sprite startgamebutton(startgametex);
// sound button
SoundBuffer buttonpressed("Assets/Music and Sounds/mouseover.ogg");
Sound buttonpressedsound(buttonpressed);
// music of main menu
Music mainmenumusic("Assets/Music and Sounds/menuMusic.mp3");
// [ADDED] Global flag for music
bool isMusicEnabled = true;
// time attack button
Texture timeattacktex("Assets/Main menu & Loading/Main Menu/mm_timeattack_normal-removebg-preview.png");
Texture timeattackpressed("Assets/Main menu & Loading/Main Menu/mm_timeattack_high-removebg-preview.png");
Sprite timeattackbutton(timeattacktex);
// High score button
Texture highscoretex("Assets/Main menu & Loading/Main Menu/mm_highscores_normal-removebg-preview.png");
Texture highscorepressed("Assets/Main menu & Loading/Main Menu/mm_highscores_high-removebg-preview.png");
Sprite highscorebutton(highscoretex);
// quit button
Texture quittex("Assets/Main menu & Loading/Main Menu/mm_quit_normal-removebg-preview.png");
Texture quitpressed("Assets/Main menu & Loading/Main Menu/mm_quit_high-removebg-preview.png");
Sprite quitbutton(quittex);
// Options button
Texture optionstex("Assets/Main menu & Loading/Main Menu/mm_options_normal-removebg-preview.png");
Texture optionspressed("Assets/Main menu & Loading/Main Menu/mm_options_high-removebg-preview.png");
Sprite optionsbutton(optionstex);
// switch user button
Texture switchusertex("Assets/Main menu & Loading/Main Menu/mm_switchuser_normal-removebg-preview.png");
Texture switchuserpressed("Assets/Main menu & Loading/Main Menu/mm_switchuser_high-removebg-preview.png");
Sprite switchuserbutton(switchusertex);
// Credits button
Texture creditstex("Assets/Main menu & Loading/Main Menu/mm_credits_normal-removebg-preview.png");
Texture creditspressed("Assets/Main menu & Loading/Main Menu/mm_credits_high-removebg-preview.png");
Sprite creditsbutton(creditstex);
// ________________________________________________________________________________
// Minowfish Textures & Sprites
Texture minowfishtex("Assets/Fish/herfish/herFish.png");
Sprite Minowfish(minowfishtex);
int MFcol = 0;
int MFrow = 0;
float MFchangedir = -1;
float MFvelocityX_AXIS = -2;
float MFvelocityY_AXIS = -2;
bool MFvisable = 1;
RectangleShape MFcollieder(Vector2f{57, 25});
// Generating A Random NUM________________________________________________
float smallfishsvelocityYaxis = getRandom(-2.f, 2.f);
CircleShape b({20});
// SMALL FISH CLASS
struct AnimatedObject
{
public:
    float changedir = 1;
    float velocityX_AXIS = -2;
    float velocityY_AXIS = -2;
    AnimatedObject(const Texture &texture, int frameWidth, int frameHeight)
        : sprite(texture, IntRect({0, 0}, {frameWidth, frameHeight}))
    {
    }
    Sprite sprite;
    int currentFrame = 0;
    float frameTime = 0.1f; // time per frame
    float timer = 0.0f;
    Vector2f velocity;
    RectangleShape shape;
    void update(int frameWidth, int frameHeight)
    {
        // Move
        sprite.move({velocityX_AXIS, velocityY_AXIS});
        changedir = (velocityX_AXIS > 0) ? -1.f : 1.f;
        sprite.setScale({0.2f * changedir, 0.2f});
        // Animate
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
// Colliders FOR Barracuda and Queen Trigger
RectangleShape Barracudacollieder(Vector2f{271, 121});
RectangleShape QueenTriggercollieder(Vector2f{260, 160});
int cu = 0;
float totaltime = 0.0f;
Vector2i mouseLocalPos;
Vector2f mouseWorldPos;
CircleShape c({10});
//________________________________________________________________________________________________________
// Yellow Fish
Texture QueenTrigerTEX("Assets/Fish/QueenTrigger/QueenTrigger.png");
Sprite QueenTrigger(QueenTrigerTEX);
int QTcol = 0;
int QTrow = 0;
float QTchangedir = -1;
float QTvelocityX_AXIS = -2;
float QTvelocityY_AXIS = -2;
RectangleShape n;
int num = 0;
// sound (repeating flag)
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
// pair<string, int> users[7];
struct Player
{
    string name = "";
    int id = 0;
    // long long highscore = 0;
};
Player players[7];
int NumberOfUsers = 0;
void SaveUsers()
{
    ofstream file("Assets/Switch User/Users_List.txt");
    for (int id = 0; id < NumberOfUsers; id++)
        file << players[id].name << '\t' << players[id].id << '\n';
    file.close();
}
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
Texture bgTexture("Assets/Select_level/gamemap_bg.jpeg");
Sprite bgSprite(bgTexture);
Texture signTexture("Assets/Select_level/gamemap_tittleboard.jpeg");
Sprite signSprite(signTexture);
Texture bannerTexture("Assets/Select_level/bord.jpeg"); // بنحمل البانر السفلي
Sprite bannerSprite(bannerTexture);
Texture myTexture("Assets/Select_level/shell_tinybtn124_normal.jpeg");    // بنحمل زرار Back to Menu - الصورة العادية
Texture myHoverTexture("Assets/Select_level/shell_tinybtn124_high.jpeg"); // بنحمل زرار Back to Menu - صورة الـ hover
Sprite mySprite(myTexture);
Texture pearlTexture("Assets/Select_level/white.jpeg");             // بنحمل صورة اللؤلؤة المقفولة
Texture pearlUnlockedTexture("Assets/Select_level/gold.jpeg");      // بنحمل صورة اللؤلؤة بعد الكسب
Texture loadBarTexture("Assets/Select_level/gamemap_loadbar.jpeg"); // بنحمل صورة الـ loading bar
Sprite pearlSprite1(pearlTexture);
Sprite pearlSprite2(pearlTexture);
Sprite pearlSprite3(pearlTexture);
Sprite ta_pearlSprite1(pearlTexture);
Sprite ta_pearlSprite2(pearlTexture);
Sprite ta_pearlSprite3(pearlTexture);

// exit
Texture quit_bg("Assets/Quit/bg_quit.png");
Sprite quit_bg_sprite(quit_bg);
Texture quit_button("Assets/Quit/Button.png");
Sprite quit_button_sprite(quit_button);
Texture quit_button_hover("Assets/Quit/Button High.png");
Sprite quit_button_hover_sprite(quit_button_hover);
Texture quit_icon("Assets/Quit/quitgame_icon.png");
Sprite quit_icon_sprite(quit_icon);

// --- متغيرات خاصة بالـ Quit Screen ---
// فلات (Flags) عشان نعرف مين اتضغط
bool quit_yes_pressed = false;
bool quit_no_pressed = false;

// التيكست (نصوص الأزرار)
Text quit_yes_text(btnFont, "Yes", 30);
Text quit_no_text(btnFont, "No!", 30);
Text quit_title_text(btnFont, "Are you sure you want to quit", 30); // <--- ضيف السطر ده
Text quit_title_text2(btnFont, "Feeding Frenzy 2?", 30);            // <--- ضيف السطر ده

// مكان وحجم اللآلئ قبل الكسب
sf::Vector2f lockedPos1 = {220.f, 87.f};
sf::Vector2f lockedScale1 = {0.08f, 0.08f};
sf::Vector2f lockedPos2 = {249.f, 133.f};
sf::Vector2f lockedScale2 = {0.06f, 0.06f};
sf::Vector2f lockedPos3 = {246.f, 170.f};
sf::Vector2f lockedScale3 = {0.06f, 0.06f};

// مكان وحجم اللآلئ بعد الكسب
sf::Vector2f unlockedPos1 = {235.f, 110.f};
sf::Vector2f unlockedScale1 = {0.036f, 0.036f};
sf::Vector2f unlockedPos2 = {261.f, 152.f};
sf::Vector2f unlockedScale2 = {0.029f, 0.029f};
sf::Vector2f unlockedPos3 = {255.f, 186.f};
sf::Vector2f unlockedScale3 = {0.028f, 0.028f};

static sf::Font font("Assets/Fonts/ARIALNBI.ttf"); // تحميل الخط المستخدم في الأزرار
static sf::Text staticTxt(font, "");
static sf::Text nowLoadingTxt(font, "");
static sf::Text menuTxt(font, "");
static sf::Text levelTxt(font, "");
static sf::Text loadingtxt(font, "");
bool level1Unlocked = true;
bool level2Unlocked = false;
bool level3Unlocked = false;
bool ta_level1Unlocked = false;
bool ta_level2Unlocked = false;
bool ta_level3Unlocked = false;
bool isTimeAttackMode = false;
// هيكل بيجمع بيانات كل لؤلؤة في حاجة واحدة
struct PearlData
{
    sf::Sprite *sprite;
    float radius;
    std::string name;
    bool *unlocked;
    sf::Vector2f lockedPos;
    sf::Vector2f lockedScale;
    sf::Vector2f unlockedPos;
    sf::Vector2f unlockedScale;
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
// =====================================================================
// SECTION 1: CORE CONFIGURATION & LAYOUT SETTINGS
// Contains the hardcoded layout values. Change these to shift the menu!
// =====================================================================
const float LAYOUT_LABEL_X = 370.f;        // Right-aligned edge for text (Sound:, Music:)
const float LAYOUT_BUBBLE_START_X = 390.f; // X position for the first column of bubbles
const float LAYOUT_ROW_SPACING = 45.f;     // Vertical spacing for Top 3 options
const float LAYOUT_COL_SPACING = 43.f;     // Horizontal spacing for bubbles (Mouse Speed/Game Detail)
const float POS_Y_TOP_OPTIONS = 170.f;     // Y start for Sound, Music, Full Screen
const float POS_Y_MOUSE_SPEED = 310.f;     // Y position for Mouse Speed
const float POS_Y_GAME_DETAIL = 360.f;     // Y position for Game Detail
// =====================================================================
// SECTION 2: GLOBAL ENGINE VARIABLES
// Contains the Window, Camera View, and Audio systems.
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
// Contains all loaded Fonts, Textures, Sprites, and the UI array.
// =====================================================================
Font globalFont;
RectangleShape screenDarkener;
// Background Layers
Texture texBgMain, texBgPlank, texTitle, texCorals;
optional<Sprite> sprBgMain, sprBgPlank, sprTitle, sprCorals;
// Standalone Labels
optional<Text> mouseLabel, detailLabel, versionLabel;
// Button Textures
Texture texUncheckedNormal, texUncheckedHover;
Texture texCheckedNormal, texCheckedHover;
Texture texDoneNormal, texDoneHover;
// Menu Data Structure
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
bool shouldCloseOptions = false; // متغير للتحكم في الخروج من الـ Options

// متغيرات الـ loading bar - عدل هنا
float barX = 258.f;     // مكان البار من الشمال
float barY = 495.f;     // مكان البار من فوق
float barWidth = 308.f; // العرض الكامل للبار
float barHeight = 28.f; // ارتفاع البار
float barSpeed = 150.f; // سرعة الـ loading
// بيانات حالة الـ loading
bool isLoading = false;
float loadProgress = 0.f;

// متغيرات الـ Fade Transition
RectangleShape fadeRect;
Clock fadeClock;
bool isFadingOut = false; // هل احنا في مرحلة التلاشي للأسود؟
bool isFadingIn = false;  // هل احنا في مرحلة الظهور من الأسود؟
float fadeAlpha = 0.f;    // درجة اللون (من 0 لـ 255)

// highscore
// --- Global Data Structures ---
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

// Done Button
Texture texHSDoneNormal("Assets/Highscore/done_normal.png");
Texture texHSDoneHover("Assets/Highscore/done_hover.png");
Sprite sprHSDonePlank(texHSDoneNormal);

// Reset Button
Texture texHSResetNormal("Assets/Highscore/shell_tinybtn124_normal.png");
Texture texHSResetHover("Assets/Highscore/shell_tinybtn124_high.jpg");
Sprite sprHSResetPlank(texHSResetNormal);

// Arrows
Texture texHSArrowNormal("Assets/Highscore/_sidescrollbtn.png");
Texture texHSArrowHover("Assets/Highscore/_sidescrollbtnhi.png");
Texture texHSArrowDown("Assets/Highscore/_sidescrollbtndown.png");
Sprite sprHSStoryArrowRight(texHSArrowNormal);
Sprite sprHSStoryArrowLeft(texHSArrowNormal);
Sprite sprHSListArrowUp(texHSArrowNormal);
Sprite sprHSListArrowDown(texHSArrowNormal);

// Fonts
Font fontHSTitle("Assets/Fonts/BernardMT.ttf");
Font fontHSMain("Assets/Fonts/Barmeno.ttf");
Font fontHSDone("Assets/Fonts/BarmenoBold.otf");

// Texts
Text textHSMainTitle(fontHSTitle, "High Scores", 48);
Text textHSStoryMode(fontHSMain, "Story Mode", 22);
Text textHSDoneButton(fontHSDone, "Done", 30);
Text textHSResetButton(fontHSMain, "Reset All Scores", 16);

// List Texts
Text textHSListRanks[VISIBLE_SCORES] = {
    {fontHSMain, "", 20}, {fontHSMain, "", 20}, {fontHSMain, "", 20}, {fontHSMain, "", 20}, {fontHSMain, "", 20}, {fontHSMain, "", 20}, {fontHSMain, "", 20}, {fontHSMain, "", 20}, {fontHSMain, "", 20}, {fontHSMain, "", 20}};
Text textHSListNames[VISIBLE_SCORES] = {
    {fontHSMain, "", 20}, {fontHSMain, "", 20}, {fontHSMain, "", 20}, {fontHSMain, "", 20}, {fontHSMain, "", 20}, {fontHSMain, "", 20}, {fontHSMain, "", 20}, {fontHSMain, "", 20}, {fontHSMain, "", 20}, {fontHSMain, "", 20}};
Text textHSListScores[VISIBLE_SCORES] = {
    {fontHSMain, "", 20}, {fontHSMain, "", 20}, {fontHSMain, "", 20}, {fontHSMain, "", 20}, {fontHSMain, "", 20}, {fontHSMain, "", 20}, {fontHSMain, "", 20}, {fontHSMain, "", 20}, {fontHSMain, "", 20}, {fontHSMain, "", 20}};

// Underline & State
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

// Colors
const Color colorTitleGreen(165, 220, 75);
const Color colorListText(240, 240, 240);
const Color colorDoneText(180, 255, 100);
const Color colorResetText(255, 255, 150);
const Color colorHoverHighlight(255, 255, 0);

// credits
Font fontcredits("Assets/Fonts/trebuc.ttf"); // الخط (فاضي في الأول)
optional<Text> creditsText;
Text textHSDoneButtoncredits(fontHSDone, "Done", 30);
Texture texHSDoneNormalcredits("Assets/Credits/done_normal.png");
Texture texHSDoneHovercredits("Assets/Credits/done_hover.png");
Sprite sprHSDonePlankcredits(texHSDoneNormalcredits);

int main()
{
    cout << "SFML 3.0 and Standard Library are working!" << endl;

    // [تعديل] فتح الشاشة في وضع Full Screen مباشرة
    window.create(VideoMode::getDesktopMode(), "Feeding Frenzy 2", State::Fullscreen);

    StartLoadingScreen();
    Clock clock;
    Clock totalClock;

    // [تعديل] تجهيز مستطيل التلاشي (Fade Overlay)
    fadeRect.setSize({WindowWidth, WindowHeight});
    fadeRect.setFillColor(Color(0, 0, 0, 0)); // أسود بشفافية صفر
    fadeRect.setPosition({0.f, 0.f});

    // [تعديل] ضبط الكاميرا: هنا بنلغي حسابات النسبة ونخليه يملأ الشاشة خالص
    view.setSize({800.f, 600.f});
    view.setCenter({400.f, 300.f});
    // ده السطر السحري اللي بيخلي الشاشة تتمدد وتملى الفراغات
    view.setViewport(sf::FloatRect({0.f, 0.f}, {1.f, 1.f}));

    window.setView(view);
    window.setFramerateLimit(60);

    // ── Loading Screen Loop ──
    while (window.isOpen())
    {
        // ... باقي كود الـ main زي ما هو ...
        deltaTime = clock.restart().asSeconds();
        float totalTime = totalClock.getElapsedTime().asSeconds();
        while (const optional event = window.pollEvent())
        {
            if (event->is<Event::Closed>())
                window.close();
            else if (const auto *resizeEvent = event->getIf<Event::Resized>())
            {
                // هنا كمان نضبط الـ Viewport عشان نضمن إنه مبيحسبش نسب
                view.setViewport(sf::FloatRect({0.f, 0.f}, {1.f, 1.f}));
            }
        }
        window.setView(view);
        window.clear(Color::Black);
        if (DrawLoadingScreen(totalTime))
            break;
    }

    // ── Main Menu Loop ──
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
{                                 // دالة تجهيز كل حاجة في البداية
    window.setFramerateLimit(45); // تثبيت السرعة على 45 فريم
    if (!swayShader.loadFromFile("sway.frag", Shader::Type::Fragment))
    {                                            // تحميل كود حركة المية
        cout << "Error loading shader!" << endl; // لو مش موجود اطبع غلط
    }
    if (!Background_Loading.loadFromFile("Assets/Main menu & Loading/Loading Screen/Images/loadbg.png"))
    {                                                               // تحميل الخلفية
        cout << "!!! Error: Could not find loadbg.png !!!" << endl; // لو مش موجودة اطبع غلط
    }
    Background_Loading_sprite.setTexture(Background_Loading); // ربط الصورة بالجسم
    Background_Loading_sprite.setPosition({0, 0});            // حطها في ركن الشاشة
    Vector2u texture_Size = Background_Loading.getSize();     // هات مقاسها الحقيقي
    float ScaleX = WindowWidth / texture_Size.x;              // احسب نسبة التكبير بالعرض
    float ScaleY = WindowHeight / texture_Size.y;             // احسب نسبة التكبير بالطول
    Background_Loading_sprite.setScale({ScaleX, ScaleY});     // كبرها تملى الشاشة
    Background_Loading.setSmooth(true);                       // نعم الحواف عشان متكسرش
    if (!Game_Icon.loadFromFile("Assets/Main menu & Loading/Loading Screen/Images/LOGO.png"))
    {                                                             // تحميل اللوجو
        cout << "!!! Error: Could not find LOGO.png !!!" << endl; // لو مش موجود اطبع غلط
    }
    float width = 300;                                          // العرض اللي عاوزه للوجو
    float height = 150;                                         // الطول اللي عاوزه للوجو
    Game_Icon_sprite.setTexture(Game_Icon);                     // اربط الصورة بالجسم
    Game_Icon_sprite.setPosition({(WindowWidth / 2) - 155, 7}); // حطه في نص الشاشة فوق
    texture_Size = Game_Icon.getSize();                         // هات مقاسه
    ScaleX = width / Game_Icon.getSize().x;                     // احسب التكبير
    ScaleY = height / Game_Icon.getSize().y;                    // احسب التكبير
    Game_Icon_sprite.setScale({ScaleX, ScaleY});                // ظبط الحجم
    Game_Icon.setSmooth(true);                                  // تنعيم الصورة
    if (!POPCAP_Logo.loadFromFile("Assets/Main menu & Loading/Loading Screen/Images/popCapLogo.png"))
    {                                                                   // تحميل لوجو الشركة
        cout << "!!! Error: Could not find popCapLogo.png !!!" << endl; // غلط لو مش موجودة
    }
    width = 120;                                                                 // عرض اللوجو الصغير
    height = 110;                                                                // طول اللوجو الصغير
    POPCAP_Logo_sprite.setTexture(POPCAP_Logo);                                  // اربط الصورة
    POPCAP_Logo_sprite.setPosition({(WindowWidth - 155), WindowHeight - 126.f}); // مكانه تحت يمين
    texture_Size = POPCAP_Logo.getSize();                                        // هات مقاسه
    ScaleX = width / POPCAP_Logo.getSize().x;                                    // احسب تكبيره
    ScaleY = height / POPCAP_Logo.getSize().y;                                   // احسب تكبيره
    POPCAP_Logo_sprite.setScale({ScaleX, ScaleY});                               // ظبط حجمه
    POPCAP_Logo.setSmooth(true);                                                 // تنعيم
    if (!LogoWood.loadFromFile("Assets/Main menu & Loading/Loading Screen/Images/sproutLogoWood.png"))
    {                                                                       // تحميل صورة الخشب
        cout << "!!! Error: Could not find sproutLogoWood.png !!!" << endl; // غلط لو مش موجودة
    }
    width = 160;                                                   // عرض الخشبة
    height = 80;                                                   // طول الخشبة
    LogoWood_sprite.setTexture(LogoWood);                          // اربط الصورة
    LogoWood_sprite.setPosition({(0 + 25), WindowHeight - 115.f}); // مكانها تحت شمال
    texture_Size = LogoWood.getSize();                             // هات مقاسها
    ScaleX = width / LogoWood.getSize().x;                         // احسب التكبير
    ScaleY = height / LogoWood.getSize().y;                        // احسب التكبير
    LogoWood_sprite.setScale({ScaleX, ScaleY});                    // ظبط الحجم
    LogoWood.setSmooth(true);                                      // تنعيم
    if (!fontTexture.loadFromFile("Assets/Fonts/LOADING.png"))
    {                                                                // تحميل صورة الحروف
        cout << "!!! Error: Could not find LOADING.png !!!" << endl; // غلط لو مش موجودة
    }
    fontTexture.setSmooth(true);                                                                                                              // تنعيم الحروف
    float cellWidth = 19.4;                                                                                                                   // عرض الحرف الواحد في الصورة
    float cellHeight = 35.5;                                                                                                                  // طول الحرف الواحد في الصورة
    Color targetColor(160, 211, 74);                                                                                                          // اللون الأخضر الفوسفوري بتاع اللعبة
    s_L.setTexture(fontTexture);                                                                                                              // اربط الحرف L بالصورة
    s_L.setTextureRect(IntRect({(int)(1 * cellWidth), (int)(3 * cellHeight)}, {(int)cellWidth, (int)cellHeight}));                            // قص مكانه
    s_L.setColor(targetColor);                                                                                                                // لونه بالأخضر
    s_L.setOrigin({cellWidth / 2.f, cellHeight / 2.f});                                                                                       // خلي السنتر في النص
    s_O.setTexture(fontTexture);                                                                                                              // اربط الحرف O
    s_O.setTextureRect(IntRect({(int)(4 * 20), (int)(3 * 35.7)}, {20, (int)35.7}));                                                           // قص مكانه
    s_O.setColor(targetColor);                                                                                                                // لونه
    s_O.setOrigin({10.f, 35.7f / 2.f});                                                                                                       // السنتر في النص
    s_A.setTexture(fontTexture);                                                                                                              // اربط الحرف A
    s_A.setTextureRect(IntRect({(int)(3 * 20), (int)(2 * cellHeight)}, {20, (int)cellHeight}));                                               // قص مكانه
    s_A.setColor(targetColor);                                                                                                                // لونه
    s_A.setOrigin({10.f, cellHeight / 2.f});                                                                                                  // السنتر في النص
    s_D.setTexture(fontTexture);                                                                                                              // اربط الحرف D
    s_D.setTextureRect(IntRect({(int)(6 * 20), (int)(2 * cellHeight)}, {20, (int)cellHeight}));                                               // قص مكانه
    s_D.setColor(targetColor);                                                                                                                // لونه
    s_D.setOrigin({10.f, cellHeight / 2.f});                                                                                                  // السنتر في النص
    s_I.setTexture(fontTexture);                                                                                                              // اربط الحرف I
    s_I.setTextureRect(IntRect({(int)(10.39 * 20 + 4), (int)(2 * cellHeight)}, {12, (int)cellHeight}));                                       // قص مكانه بدقة
    s_I.setColor(targetColor);                                                                                                                // لونه
    s_I.setOrigin({6.f, cellHeight / 2.f});                                                                                                   // السنتر في النص
    s_N.setTexture(fontTexture);                                                                                                              // اربط الحرف N
    s_N.setTextureRect(IntRect({(int)(3 * 20), (int)(3 * cellHeight)}, {20, (int)cellHeight}));                                               // قص مكانه
    s_N.setColor(targetColor);                                                                                                                // لونه
    s_N.setOrigin({10.f, cellHeight / 2.f});                                                                                                  // السنتر في النص
    s_G.setTexture(fontTexture);                                                                                                              // اربط الحرف G
    s_G.setTextureRect(IntRect({174, (int)(2 * cellHeight)}, {18, (int)cellHeight}));                                                         // قص مكانه
    s_G.setColor(targetColor);                                                                                                                // لونه
    s_G.setOrigin({9.f, cellHeight / 2.f});                                                                                                   // السنتر في النص
    bubbleSprite.setTexture(bubbleTexture);                                                                                                   // اربط فقاعة الحروف
    bubbleSprite.setScale({0.7f, 0.7f});                                                                                                      // صغر حجمها شوية
    bubbleSprite.setOrigin({bubbleSprite.getLocalBounds().size.x / 2.f, bubbleSprite.getLocalBounds().size.y / 2.f});                         // سنترها
    width_bubble_global = bubbleSprite.getGlobalBounds().size.x;                                                                              // احفظ عرضها للعمليات
    bubbleTexture.setSmooth(true);                                                                                                            // تنعيم
    width = 320.f;                                                                                                                            // عرض شريط التحميل
    height = 50.f;                                                                                                                            // طول شريط التحميل
    loadBar_empty_sprite.setTexture(loadBar_empty);                                                                                           // اربط برواز الشريط
    loadBar_empty_sprite.setOrigin({loadBar_empty_sprite.getLocalBounds().size.x / 2.f, loadBar_empty_sprite.getLocalBounds().size.y / 2.f}); // سنتره
    loadBar_empty_sprite.setPosition({WindowWidth / 2.f, WindowHeight - 85.f});                                                               // حطه تحت في النص
    texture_Size = loadBar_empty.getSize();                                                                                                   // مقاسه الحقيقي
    ScaleX = width / loadBar_empty.getSize().x;                                                                                               // احسب تكبير العرض
    ScaleY = height / loadBar_empty.getSize().y;                                                                                              // احسب تكبير الطول
    loadBar_empty_sprite.setScale({ScaleX, ScaleY});                                                                                          // ظبط الحجم
    loadBar_empty.setSmooth(true);                                                                                                            // تنعيم
    loadBar_sprite.setTexture(loadBar);                                                                                                       // اربط الشريط المليان
    loadBar_sprite.setOrigin({loadBar_sprite.getLocalBounds().size.x / 2.f, loadBar_sprite.getLocalBounds().size.y / 2.f});                   // سنتره
    loadBar_sprite.setPosition({WindowWidth / 2.f, WindowHeight - 85.f});                                                                     // حطه في نفس مكان البرواز
    loadBar_sprite.setScale({width / loadBar.getSize().x, height / loadBar.getSize().y});                                                     // ظبط حجمه
    loadBar.setSmooth(true);                                                                                                                  // تنعيم
    bubbleSmallTex.setSmooth(true);                                                                                                           // تنعيم الفقاقيع الصغيرة
    spark.setSmooth(true);                                                                                                                    // تنعيم الشرارات
}
void StartLoadingScreen()
{                     // دالة البدء
    loading_screen(); // شغل دالة التحميل
}
void UpdateLoadingScreen()
{ // دالة التحديث (فاضية حالياً)
}
bool DrawLoadingScreen(float totalTime)
{                                                           // دالة الرسم (بتتكرر كل فريم)
    window.clear(Color::Black);                             // امسح الشاشة وخليها سودة
    swayShader.setUniform("time", totalTime);               // ابعت الوقت للشيدر عشان المية تتحرك
    Background_Loading.setSmooth(true);                     // تأكيد التنعيم
    window.draw(Background_Loading_sprite, &swayShader);    // ارسم الخلفية وعليها حركة المية
    window.draw(Game_Icon_sprite);                          // ارسم لوجو اللعبة
    window.draw(POPCAP_Logo_sprite);                        // ارسم لوجو الشركة
    window.draw(LogoWood_sprite);                           // ارسم الخشبة
    float loadingStartTime = 1.0f;                          // وقت بداية التحميل (بعد ثانية)
    float progress = (totalTime - loadingStartTime) / 5.0f; // النسبة (على مدار 5 ثواني)
    if (progress < 0)
        progress = 0; // لو لسه مجاش الوقت خليها صفر
    if (progress > 1.0f)
        progress = 1.0f;                                                                // لو خلصت ثبتها على 100%
    Vector2u textureSize = loadBar.getSize();                                           // مقاس صورة الشريط
    int currentWidth = (int)(textureSize.x * progress);                                 // العرض اللي المفروض يظهر
    loadBar_sprite.setTextureRect(IntRect({0, 0}, {currentWidth, (int)textureSize.y})); // قص الجزء المليان بس
    window.draw(loadBar_empty_sprite);                                                  // ارسم البرواز الفاضي
    window.draw(loadBar_sprite);                                                        // ارسم الجزء المليان فوقه
    startX = WindowWidth / 2.0f - (3.5f * spacing) + 20.f;                              // سنتر الكلمة بالعرض
    Sprite *letters[] = {&s_L, &s_O, &s_A, &s_D, &s_I, &s_N, &s_G};                     // لستة بالحروف
    for (int i = 0; i < 7; i++)
    {                                                            // لف على الـ 7 حروف
        float startDelay = 1.5f;                                 // تأخير بداية ظهور الحروف
        float letterDelay = i * 0.4f;                            // تأخير بين كل حرف والتاني
        float letterTime = totalTime - startDelay - letterDelay; // الوقت الخاص بالحرف ده
        float popScale = 0.0f;                                   // الحجم بيبدأ من صفر
        if (letterTime > 0)
        {                                                                                    // لو جه ميعاد ظهوره
            popScale = (letterTime < 0.4f) ? sin(letterTime * (3.14f / 0.4f)) * 1.2f : 1.0f; // حركة الـ Pop (يكبر ويرجع)
            if (popScale < 1.0f && letterTime > 0.2f)
                popScale = 1.0f; // تثبيته بعد الحركة
        }
        float curX = startX + (i * spacing);                                                  // مكانه الأفقي
        float curY = (WindowHeight / 2.0f - 40.f) + sin(totalTime * 4.0f + i * 0.8f) * 12.0f; // حركة الموج لفوق وتحت
        bubbleSprite.setScale({0.7f * popScale, 0.7f * popScale});                            // حجم الفقاعة مع الـ Pop
        letters[i]->setScale({0.92f * popScale, 0.92f * popScale});                           // حجم الحرف مع الـ Pop
        bubbleSprite.setPosition({curX, curY});                                               // مكان الفقاعة
        letters[i]->setPosition({curX, curY});                                                // مكان الحرف
        if (progress < 1.0f && popScale > 0)
        {                              // لو التحميل شغال ارسمهم
            window.draw(bubbleSprite); // ارسم الفقاعة
            window.draw(*letters[i]);  // ارسم الحرف
        }
    }
    if (progress >= 1.0f && !exploded)
    {                    // لو خلص تحميل ومفرقعناش
        exploded = true; // ابدأ الفرقعة
        for (int i = 0; i < 35; i++)
        {                                        // جهز 35 جزيء
            int idx = i / 5;                     // وزعهم على الـ 7 حروف
            float pX = startX + (idx * spacing); // نقطة انطلاقهم
            float pY = (WindowHeight / 2.0f - 40.f);
            sparks[i].active = true; // فعل الشرارة
            sparks[i].x = pX;
            sparks[i].y = pY;                                   // حطها في مكانها
            sparks[i].vx = (float)(rand() % 60 - 20) / 15.0f;   // سرعة أفقية عشوائية
            sparks[i].vy = -((float)(rand() % 30 + 10)) / 6.0f; // قذفة لفوق
            sparks[i].alpha = 255.0f;                           // واضحة جداً
            letterBubbles[i].active = true;                     // فعل فقاعة الفرقعة
            letterBubbles[i].x = pX;
            letterBubbles[i].y = pY;
            letterBubbles[i].vx = (float)(rand() % 60 - 20) / 18.0f;
            letterBubbles[i].vy = -((float)(rand() % 15 + 5)) / 5.0f;
            letterBubbles[i].alpha = 200.0f;
        }
    }
    if (exploded && !particlesFinished)
    {                           // لو الفرقعة شغالة
        bool anyActive = false; // علامة لو فيه حاجة لسه بتتحرك
        for (int i = 0; i < 35; i++)
        { // لف على كل الجزيئات
            if (sparks[i].active)
            {                                // لو الشرارة شغالة
                anyActive = true;            // لسه فيه حركة
                sparks[i].x += sparks[i].vx; // حركها أفقي
                sparks[i].vy += 0.2f;        // تأثير الجاذبية
                sparks[i].y += sparks[i].vy; // حركها رأسي
                sparks[i].alpha -= 6.f;      // قلل وضوحها
                if (sparks[i].alpha <= 0)
                {                             // لو اختفت تماماً
                    sparks[i].alpha = 0;      // صفرها
                    sparks[i].active = false; // عطلها
                }
                else
                {
                    sparkSprite.setPosition({sparks[i].x, sparks[i].y});                  // حط صورة الشرارة
                    sparkSprite.setColor(Color(255, 255, 255, (uint8_t)sparks[i].alpha)); // ظبط شفافيتها
                    sparkSprite.setScale({0.55f, 0.55f});                                 // حجمها
                    window.draw(sparkSprite);                                             // ارسمها
                }
            }
            if (letterBubbles[i].active)
            {                                              // لو فقاعة الفرقعة شغالة
                anyActive = true;                          // لسه فيه حركة
                letterBubbles[i].x += letterBubbles[i].vx; // حرك أفقياً
                letterBubbles[i].y += letterBubbles[i].vy; // حرك رأسياً
                letterBubbles[i].alpha -= 7.f;             // قلل وضوحها
                if (letterBubbles[i].alpha <= 0)
                { // لو اختفت
                    letterBubbles[i].alpha = 0;
                    letterBubbles[i].active = false;
                }
                else
                {
                    bubbleSmallSprite.setPosition({letterBubbles[i].x, letterBubbles[i].y});           // حط الصورة
                    bubbleSmallSprite.setColor(Color(255, 255, 255, (uint8_t)letterBubbles[i].alpha)); // الشفافية
                    window.draw(bubbleSmallSprite);                                                    // ارسم
                }
            }
        }
        if (!anyActive)
        {                             // لو مفيش حاجة تانية بتتحرك
            particlesFinished = true; // كدا خلصنا فرقعة
        }
    }
    timerLeft += deltaTime;               // زود تايمر الشمال
    timerRight += deltaTime;              // زود تايمر اليمين
    bubbleTimer += deltaTime;             // زود تايمر الجو العام
    auto spawnBubble = [&](int ventIdx) { // دالة "تفريخ" فقاقيع من المرجان
        for (int i = 0; i < 60; i++)
        {
            if (!ventBubbles[i].active)
            {                                                                    // دور على مكان فاضي في المصفوفة
                ventBubbles[i].active = true;                                    // فعلها
                ventBubbles[i].isVent = true;                                    // قول إنها من المرجان
                ventBubbles[i].alpha = 255.0f;                                   // واضحة
                ventBubbles[i].x = ventPositions[ventIdx].x + (rand() % 12 - 6); // مكان عشوائي بسيط
                ventBubbles[i].y = ventPositions[ventIdx].y;                     // من عند الفتحة
                ventBubbles[i].vx = (float)(rand() % 10 - 5) / 100.0f;           // حركة أفقية ضعيفة
                ventBubbles[i].vy = -((float)(rand() % 20 + 20)) / 20.0f;        // طالعة لفوق بسرعة
                return true;
            }
        }
        return false;
    };
    if (timerLeft >= 0.8f)
    {
        if (spawnBubble(0))
            timerLeft = 0.0f;
    } // طلع فقاعة كل 0.8 ثانية من الشمال
    if (timerRight >= 0.8f)
    {
        if (spawnBubble(1))
            timerRight = 0.0f;
    } // طلع فقاعة كل 0.8 ثانية من اليمين
    if (bubbleTimer >= 1.5f)
    { // طلع فقاعة بالعرض كل 1.5 ثانية
        for (int i = 0; i < 60; i++)
        {
            if (!ventBubbles[i].active)
            {
                ventBubbles[i].active = true;
                ventBubbles[i].isVent = false; // مش من المرجان
                ventBubbles[i].alpha = 255.0f;
                ventBubbles[i].x = 810.f;                                // من بره الشاشة يمين
                ventBubbles[i].y = (float)(rand() % 601);                // ارتفاع عشوائي
                ventBubbles[i].vx = -((float)(rand() % 30 + 15) / 10.f); // raيحة شمال
                ventBubbles[i].vy = 0.0f;
                bubbleTimer = 0.0f;
                break;
            }
        }
    }
    for (int i = 0; i < 60; i++)
    { // تحريك ورسم كل فقاقيع الجو
        if (ventBubbles[i].active)
        {
            ventBubbles[i].x += ventBubbles[i].vx; // حرك أفقي
            ventBubbles[i].y += ventBubbles[i].vy; // حرك رأسي
            if (ventBubbles[i].isVent)
            {                                                          // لو فقاعة مرجان
                ventBubbles[i].x += sin(totalTime * 4.0f + i) * 0.25f; // خليها تترعش وهي طالعة
                if (ventBubbles[i].y < 200.f)
                    ventBubbles[i].alpha -= 8.0f; // خليها تختفي قرب السطح
                if (ventBubbles[i].alpha <= 0 || ventBubbles[i].y < 10.f)
                    ventBubbles[i].active = false; // لو خلصت عطلها
            }
            else
            { // لو فقاعة بالعرض
                if (ventBubbles[i].x < -50.f)
                    ventBubbles[i].active = false; // لو خرجت من الشاشة عطلها
            }
            if (ventBubbles[i].active)
            {                                                                                    // لو لسه نشطة ارسمها
                bubbleSmallSprite.setPosition({ventBubbles[i].x, ventBubbles[i].y});             // حط المكان
                bubbleSmallSprite.setColor(Color(255, 255, 255, (uint8_t)ventBubbles[i].alpha)); // حط الشفافية
                float s = ventBubbles[i].isVent ? (0.8f + (i % 3) * 0.2f) : 0.65f;               // مقاسات متنوعة
                bubbleSmallSprite.setScale({s, s});                                              // ظبط الحجم
                window.draw(bubbleSmallSprite);                                                  // ارسم
            }
        }
    }
    // --- منطق الـ Fade Out والخروج ---
    if (particlesFinished)
    {
        if (!transitionStarted)
        {
            transitionStarted = true;
            fadeClock.restart();
            fadeAlpha = 0.f;
        }

        // نفس منطق الـ Loading Screen بالظبط
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

    // Fade In للمينيو
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

                    // --- زرار Start Game / Time Attack ---
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
                    // --- زرار Switch User ---
                    if (switchuserbutton.getGlobalBounds().contains(mousePos))
                    {
                        FadeOutToBlack();
                        SwitchUser();
                        FadeInFromBlack();
                    }
                    // --- زرار Options ---
                    if (optionsbutton.getGlobalBounds().contains(mousePos))
                    {
                        FadeOutToBlack();
                        OptionsMenu();
                        FadeInFromBlack();
                    }
                    // --- زرار Quit ---
                    if (quitbutton.getGlobalBounds().contains(mousePos))
                    {
                        FadeOutToBlack();
                        QuitGame();
                        FadeInFromBlack();
                    }
                    if (highscorebutton.getGlobalBounds().contains(mousePos))
                    {
                        // 1. عمل Fade Out باستخدام الدالة الجديدة
                        FadeOutToBlack();

                        // 2. ننتقل للشاشة الجديدة
                        Highscore();

                        // 3. لما نرجع، نعمل Fade In تاني
                        FadeInFromBlack();
                    }
                    if (creditsbutton.getGlobalBounds().contains(mousePos))
                    {
                        // 1. عمل Fade Out باستخدام الدالة الجديدة
                        FadeOutToBlack();

                        // 2. ننتقل للشاشة الجديدة
                        Credits();

                        // 3. لما نرجع، نعمل Fade In تاني
                        FadeInFromBlack();
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
    if (!swayShader.loadFromFile("sway.frag", Shader::Type::Fragment))
    {                                            // تحميل كود حركة المية
        cout << "Error loading shader!" << endl; // لو مش موجود اطبع غلط
    }

    smallfishs.clear();
    // [MODIFIED] Check if music is enabled before playing
    if (isMusicEnabled && mainmenumusic.getStatus() != Music::Status::Playing)
        mainmenumusic.play();
    mainmenumusic.setLooping(true);

    srand(time(0));
    // creating small fish objects
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

        // obj.changedir = 1;
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

    window.setFramerateLimit(30);
    // set up Textures smooth
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

    // ... (كود الموسيقى والـ random) ...

    // setting up sprites
    background.setOrigin(background.getLocalBounds().size / 2.0f);
    // الخطأ هنا كان using window.getSize().x بدل WindowWidth
    background.setPosition(Vector2f({WindowWidth / 2.0f, WindowHeight / 2.0f}));
    // ... باقي الكود ...

    logosp.setOrigin(logosp.getLocalBounds().size / 2.0f);
    logosp.setPosition(Vector2f({WindowWidth / 2.0f, WindowHeight / 2.0f - 200})); // تم التعديل

    // ... (كود الـ Colliders) ...

    Greenfish.setPosition(Vector2f({WindowWidth / 2.0f - 300, WindowHeight / 2.0f})); // تم التعديل
    // ... (باقي كود الـ Greenfish) ...

    startgamebutton.setOrigin(startgamebutton.getLocalBounds().size / 2.0f);
    startgamebutton.setPosition(Vector2f({WindowWidth / 2.0f, WindowHeight / 2.0f - 50})); // تم التعديل

    timeattackbutton.setOrigin(timeattackbutton.getLocalBounds().size / 2.0f);
    timeattackbutton.setPosition(Vector2f({WindowWidth / 2.0f + 10, WindowHeight / 2.0f + 50})); // تم التعديل

    highscorebutton.setOrigin(highscorebutton.getLocalBounds().size / 2.0f);
    highscorebutton.setPosition(Vector2f({WindowWidth / 2.0f + -150, WindowHeight / 2.0f + 140})); // تم التعديل

    optionsbutton.setOrigin(optionsbutton.getLocalBounds().size / 2.0f);
    optionsbutton.setPosition(Vector2f({WindowWidth / 2.0f + 120, WindowHeight / 2.0f + 150})); // تم التعديل

    quitbutton.setOrigin(quitbutton.getLocalBounds().size / 2.0f);
    quitbutton.setPosition(Vector2f({WindowWidth / 2.0f, WindowHeight / 2.0f + 230})); // تم التعديل

    switchuserbutton.setOrigin(switchuserbutton.getLocalBounds().size / 2.0f);
    switchuserbutton.setPosition(Vector2f({WindowWidth / 2.0f + 300, WindowHeight / 2.0f + 250})); // تم التعديل

    creditsbutton.setOrigin(creditsbutton.getLocalBounds().size / 2.0f);
    creditsbutton.setPosition(Vector2f({WindowWidth / 2.0f - 300, WindowHeight / 2.0f + 240})); // تم التعديل

    // ... (كود الـ Barracuda) ...
    Barracuda.setPosition(Vector2f({WindowWidth / 2.f - 700, WindowHeight / 2.f - 200})); // تم التعديل

    // ... (كود الـ QueenTrigger) ...
    QueenTrigger.setPosition({1450, 360}); // هنا المفروض تعدله عشان يظهر جوه الشاشة، مثلاً:
    // QueenTrigger.setPosition({ WindowWidth - 100.f, WindowHeight / 2.f });
    // (لكن سيب زي ما هو لو عايزاه يظهر من بره)
}

void UpdateMainMenu()
{
    float smallfishsvelocityYaxis = getRandom(-2.f, 2.f);
    // COLLIDERS UPDATE
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
    // mouse position
    Vector2i mouseLocalPos = Mouse::getPosition(window); // Get the mouse position relative to the window
    Vector2f mouseWorldPos = window.mapPixelToCoords(mouseLocalPos);

    // MOVING THE SMALL FISHES AND THIER ANIMATION AND CHANGING DIRECTION IF THEY HIT THE WALL
    for (auto &obj : smallfishs)
    {
        obj.shape.setPosition({obj.sprite.getPosition().x, obj.sprite.getPosition().y});
        obj.update(286, 126);

        // FIX: use <= / >= with game-space bounds, NOT window.getPosition()
        float posX = obj.sprite.getPosition().x;
        float posY = obj.sprite.getPosition().y;

        if (posX <= -150.f || posX >= WindowWidth + 150.f)
        {
            obj.velocityX_AXIS *= -1;
            // obj.changedir *= -1;
            // obj.sprite.setScale({0.2f * obj.changedir, 0.2f});
            // nudge fish back inside to prevent repeated triggers
            if (posX <= -150.f)
                obj.sprite.setPosition({-149.f, posY});
            else
                obj.sprite.setPosition({WindowWidth + 149.f, posY});
        }

        if (posY <= -100.f || posY >= WindowHeight + 100.f)
        {
            obj.velocityY_AXIS *= -1;
            // obj.velocityX_AXIS *= -1;
            // obj.changedir *= -1;
            // obj.sprite.setScale({0.2f * obj.changedir, 0.2f});
            if (posY <= -100.f)
                obj.sprite.setPosition({posX, -99.f});
            else
                obj.sprite.setPosition({posX, WindowHeight + 99.f});
        }
    }
    // GreenfishAnimation();
    PlayingSound(true);
    // MinowFishanimation();
    ChangingButtonShape();
    BarracudaFishanimation();
    QueenTriggerFish();
}

void ChangingButtonShape()
{
    Vector2i mouseLocalPos = Mouse::getPosition(window);
    Vector2f mouseWorldPos = window.mapPixelToCoords(mouseLocalPos, view);

    // زرار Start Game
    if (startgamebutton.getGlobalBounds().contains(mouseWorldPos))
        startgamebutton.setTexture(startgamepressed);
    else
        startgamebutton.setTexture(startgametex);

    // زرار Time Attack
    if (timeattackbutton.getGlobalBounds().contains(mouseWorldPos))
        timeattackbutton.setTexture(timeattackpressed);
    else
        timeattackbutton.setTexture(timeattacktex);

    // زرار High Score
    if (highscorebutton.getGlobalBounds().contains(mouseWorldPos))
        highscorebutton.setTexture(highscorepressed);
    else
        highscorebutton.setTexture(highscoretex);

    // زرار Options
    if (optionsbutton.getGlobalBounds().contains(mouseWorldPos))
        optionsbutton.setTexture(optionspressed);
    else
        optionsbutton.setTexture(optionstex);

    // زرار Quit
    if (quitbutton.getGlobalBounds().contains(mouseWorldPos))
        quitbutton.setTexture(quitpressed);
    else
        quitbutton.setTexture(quittex);

    // زرار Switch User
    if (switchuserbutton.getGlobalBounds().contains(mouseWorldPos))
        switchuserbutton.setTexture(switchuserpressed);
    else
        switchuserbutton.setTexture(switchusertex);

    // زرار Credits
    if (creditsbutton.getGlobalBounds().contains(mouseWorldPos))
        creditsbutton.setTexture(creditspressed);
    else
        creditsbutton.setTexture(creditstex);
}

void DrawMainMenuBackground()
{
    window.setView(view);
    window.clear();
    swayShader.setUniform("time", totaltime); // تحديث الوقت للـ Shader
    window.draw(background, &swayShader);
    // window.draw(Greenfish);
    // window.draw(MFcollieder);
    // window.draw(QueenTriggercollieder);
    window.draw(QueenTrigger);
    // window.draw(Minowfish);
    // MainMenuFishAnimation();
    for (auto &obj : smallfishs)
        window.draw(obj.sprite);
    // window.draw(n);
    // window.draw(Barracudacollieder);
    window.draw(Barracuda);
    // window.draw(b);
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
    Greenfish.move({GFvelocityX_AXIS, GFvelocityY_AXIS});
    if (Greenfish.getPosition().x == window.getSize().x / 2.0f - 580 || Greenfish.getPosition().x == window.getSize().x / 2.0f + 580)
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
        {
            GFeat = 0;
        }
        if (counter == 4)
        {
            Minowfish.setPosition({3000, 3000});
        }
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
void PlayingSound(bool isMainMenu)
{
    Vector2i mouseLocalPos = Mouse::getPosition(window);
    Vector2f mouseWorldPos = window.mapPixelToCoords(mouseLocalPos);

    bool isHovering = false;

    // 1. أزرار الـ Main Menu
    if (isMainMenu)
    {
        isHovering = (startgamebutton.getGlobalBounds().contains(mouseWorldPos) ||
                      timeattackbutton.getGlobalBounds().contains(mouseWorldPos) ||
                      highscorebutton.getGlobalBounds().contains(mouseWorldPos) ||
                      optionsbutton.getGlobalBounds().contains(mouseWorldPos) ||
                      quitbutton.getGlobalBounds().contains(mouseWorldPos) ||
                      switchuserbutton.getGlobalBounds().contains(mouseWorldPos) ||
                      creditsbutton.getGlobalBounds().contains(mouseWorldPos) || YesButton.getGlobalBounds().contains(mouseWorldPos) || NoButton.getGlobalBounds().contains(mouseWorldPos) || sprHSDonePlankcredits.getGlobalBounds().contains(mouseWorldPos) || sprHSResetPlank.getGlobalBounds().contains(mouseWorldPos));
    }
    else
    {
        isHovering = NewButton.getGlobalBounds().contains(mouseWorldPos) || SelectButton.getGlobalBounds().contains(mouseWorldPos) || DeleteButton.getGlobalBounds().contains(mouseWorldPos) || quit_yes_text.getGlobalBounds().contains(mouseWorldPos) || quit_no_text.getGlobalBounds().contains(mouseWorldPos) || mySprite.getGlobalBounds().contains(mouseWorldPos) || (OptionButtons[10].checkbox.has_value() && OptionButtons[10].checkbox->getGlobalBounds().contains(mouseWorldPos) || YesButton.getGlobalBounds().contains(mouseWorldPos) || NoButton.getGlobalBounds().contains(mouseWorldPos) || sprHSDonePlankcredits.getGlobalBounds().contains(mouseWorldPos) || sprHSResetPlank.getGlobalBounds().contains(mouseWorldPos));
    }
    // لو وقف على أي زرار -> شغل الصوت مرة واحدة
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
        Minowfish.move({MFvelocityX_AXIS, 0});
        if (Minowfish.getPosition().x == window.getSize().x / 2.0f - 580 || Minowfish.getPosition().x == window.getSize().x / 2.0f + 580)
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
void BarracudaFishanimation()
{
    Barracuda.move({BFvelocityX_AXIS, 0});
    BFrow = 0;
    if (Barracuda.getPosition().x == window.getSize().x / 2.0f - 580 || Barracuda.getPosition().x == window.getSize().x / 2.0f + 580)
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
        {
            BFcol = 0;
        }
        if (counter == 4)
        {
            Minowfish.setPosition({3000, 3000});
        }
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
    {
        BFvelocityY_AXIS *= -1;
    }
}
void QueenTriggerFish()
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
    else if (QueenTriggercollieder.getGlobalBounds().findIntersection(smallfishs[num].sprite.getGlobalBounds()) || QueenTriggercollieder.getGlobalBounds().findIntersection(smallfishs[2].sprite.getGlobalBounds()))
    {
        if (cu < 1)
        {
            QTcol = 0;
        }
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

            else if (const auto* resizeEvent = event->getIf<Event::Resized>())
            {
                view.setViewport(FloatRect({ 0.f, 0.f }, { 1.f, 1.f }));
                window.setView(view);
            }

            if (auto mouseEvent = event->getIf<Event::MouseButtonReleased>())
                if (mouseEvent->button == Mouse::Button::Left)
                {
                    if (isLoading) continue;

                    Vector2f mf = window.mapPixelToCoords(Mouse::getPosition(window), view);

                    if (mySprite.getGlobalBounds().contains(mf))
                    {
                        return;
                    }

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
void centerText(Text& text)
{
    FloatRect b = text.getLocalBounds();
    text.setOrigin({ b.position.x + b.size.x / 2.f, b.position.y + b.size.y / 2.f });
    text.setPosition({ 415.f, 545.f });
}
void StartSelectLevel()
{
    isLoading = false;
    loadProgress = 0.f;
    if (!bgTexture.loadFromFile("Assets/Select_level/gamemap_bg.jpeg"))
    {
        std::cerr << "Error: Could not load gamemap_bg.jpeg" << std::endl;
    }
    bgSprite.setTexture(bgTexture);
    bgTexture.setSmooth(true);
    if (!signTexture.loadFromFile("Assets/Select_level/gamemap_tittleboard.jpeg"))
    {
        std::cerr << "Error: Could not load gamemap_tittleboard.jpeg" << std::endl;
    }
    signTexture.setSmooth(true);
    signSprite.setTexture(signTexture);
    signSprite.setPosition({ 10.f, 10.f });
    if (!bannerTexture.loadFromFile("Assets/Select_level/bord.jpeg"))
    {
        std::cerr << "Error: Could not load bord.jpeg" << std::endl;
    }
    bannerTexture.setSmooth(true);
    bannerSprite.setTexture(bannerTexture);
    bannerSprite.setPosition({ 1.f, 445.f });
    if (!myTexture.loadFromFile("Assets/Select_level/shell_tinybtn124_normal.jpeg"))
    {
        std::cerr << "Error: Could not load shell_tinybtn124_normal.jpeg" << std::endl;
    }
    if (!myHoverTexture.loadFromFile("Assets/Select_level/shell_tinybtn124_high.jpeg"))
    {
        std::cerr << "Error: Could not load shell_tinybtn124_high.jpeg" << std::endl;
    }
    myTexture.setSmooth(true);
    myHoverTexture.setSmooth(true);
    mySprite.setTexture(myTexture);
    mySprite.setPosition({ 350.f, 565.f });
    if (!pearlTexture.loadFromFile("Assets/Select_level/white.jpeg"))
    {
        std::cerr << "Error: Could not load white.jpeg" << std::endl;
    }
    pearlTexture.setSmooth(true);
    if (!pearlUnlockedTexture.loadFromFile("Assets/Select_level/gold.jpeg"))
    {
        std::cerr << "Error: Could not load gold.jpeg" << std::endl;
    }
    pearlUnlockedTexture.setSmooth(true);
    if (!loadBarTexture.loadFromFile("Assets/Select_level/gamemap_loadbar.jpeg"))
    {
        std::cerr << "Error: Could not load gamemap_loadbar.jpeg" << std::endl;
    }
    loadBarTexture.setSmooth(true);
    if (!font.openFromFile("Assets/Fonts/ARIALNBI.ttf"))
    {
        cout << "Error loading font!\n";
    }
    font.setSmooth(true);
    staticTxt = Text(font, "NEW GAME", 25);
    staticTxt.setPosition({ 356.f, 493.f });
    menuTxt = Text(font, "menu", 20);
    menuTxt.setPosition({ 390.f, 570.f });
    levelTxt = Text(font, "choose level", 24);
    centerText(levelTxt);
    loadingtxt = Text(font, "Now Loading...", 24);
    loadingtxt.setPosition({ 356.f, 493.f });
    pearls[0] = { &pearlSprite1, 25.f, "Level 1", &level1Unlocked, {205.f, 75.f}, {0.13f, 0.13f}, {220.f, 100.f}, {0.13f, 0.13f} };
    pearls[1] = { &pearlSprite2, 25.f, "Level 2", &level2Unlocked, {235.f, 125.f}, {0.15f, 0.15f}, {248.f, 148.f}, {0.15f, 0.15f} };
    pearls[2] = { &pearlSprite3, 25.f, "Level 3", &level3Unlocked, {230.f, 165.f}, {0.15f, 0.15f}, {242.f, 188.f}, {0.15f, 0.15f} };
    pearls[3] = { &pearlSprite4, 25.f, "Level 4", &level4Unlocked, {210.f, 200.f}, {0.15f, 0.15f}, {224.f, 222.f}, {0.15f, 0.15f} };
}
void UpdateSelectLevel(float dt)
{
    PearlData* activePearls = isTimeAttackMode ? ta_pearls : pearls;

    PlayingSound(true);
    Vector2f mf = window.mapPixelToCoords(Mouse::getPosition(window), view);
    levelTxt.setString("choose level");
    centerText(levelTxt);

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

    if (mySprite.getGlobalBounds().contains(mf))
    {
        mySprite.setTexture(myHoverTexture);
        levelTxt.setString("back to menu");
        centerText(levelTxt);
    }
    else
    {
        mySprite.setTexture(myTexture);
    }

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
            return;
        }
    }
}
void DrawSelectLevel()
{
    window.setView(view);
    window.clear();
    window.draw(bgSprite);
    window.draw(signSprite);
    window.draw(bannerSprite);
    if (!isTimeAttackMode) {
    window.draw(pearlSprite1);
    window.draw(pearlSprite2);
    window.draw(pearlSprite3);
} else {
    window.draw(ta_pearlSprite1);
    window.draw(ta_pearlSprite2);
    window.draw(ta_pearlSprite3);
}
    window.draw(mySprite);
    window.draw(menuTxt);
    window.draw(levelTxt);
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
    else
    {
        window.draw(staticTxt);
    }
    window.display();
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
        for (auto &obj : smallfishs)
        {
            obj.shape.setPosition({obj.sprite.getPosition().x, obj.sprite.getPosition().y});
            obj.update(286, 126);

            float posX = obj.sprite.getPosition().x;
            if (posX <= -150.f || posX >= WindowWidth + 150.f)
            {
                obj.velocityX_AXIS *= -1;
                // obj.changedir *= -1;
                // obj.sprite.setScale({0.2f * obj.changedir, 0.2f});
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
    CreateButton(Full, FullTex, "Assets/Switch User/BG.png", X, Y, 0.2, 0.2); // Background of the SwitchUser pop-up window
    X = WindowWidth * 0.25f, Y = WindowHeight * 0.78f;
    CreateButton(NewButton, NewButtonTex, "Assets/Switch User/Button.png", X, Y, 1.2, 1.2);
    if (!NewButtonHLTex.loadFromFile("Assets/Switch User/Button High.png"))
        cout << "Failed to load: ButtonHL" << "\n";
    X = WindowWidth * 0.5f, Y = WindowHeight * 0.78f;
    CreateButton(SelectButton, SelectButtonTex, "Assets/Switch User/Button.png", X, Y, 1.2, 1.2);
    if (!SelectButtonHLTex.loadFromFile("Assets/Switch User/Button High.png"))
        cout << "Failed to load: ButtonHL" << "\n";
    X = WindowWidth * 0.75f, Y = WindowHeight * 0.78f;
    CreateButton(DeleteButton, DeleteButtonTex, "Assets/Switch User/Button.png", X, Y, 1.2, 1.2);
    if (!DeleteButtonHLTex.loadFromFile("Assets/Switch User/Button High.png"))
        cout << "Failed to load: ButtonHL" << "\n";
    if (!btnFont.openFromFile("Assets/Fonts/BarmenoBold.otf"))
        cout << "Can't load font\n";
    SetupButtonText(NewText, "New", NewButton);
    SetupButtonText(SelectText, "Select", SelectButton);
    SetupButtonText(DeleteText, "Delete", DeleteButton);
    X = WindowWidth * 0.5f, Y = WindowHeight * 0.175f;
    CreateButton(Title, TitleTex, "Assets/Switch User/shell_chooseuser_hdr.png", X, Y, 1.5, 1.5);
    // ----------- Checking to see if we already Have Users------------------
    for (auto &player : players)
        player = {"", 0};
    ifstream loadusers("Assets/Switch User/Users_List.txt");
    string name;
    int id;
    while (loadusers >> name >> id and NumberOfUsers < MaxNumberOfUsers)
    {
        players[NumberOfUsers].name = name;
        players[NumberOfUsers].id = id;
        NumberOfUsers++;
    }
    loadusers.close();
    RefreshUsersList(); // Refreshing the array of users after loading them from the file
    // ----------- HighLight if user is choosen -------------------
    SelectUserHL.setSize({WindowWidth * 0.35f, 45.f});
    SelectUserHL.setFillColor(Color(180, 220, 255, 150));
    SelectUserHL.setOrigin({SelectUserHL.getSize().x / 2.f, 0.f});
}
// ------------------------ Template ------------------------
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
// ------------------------ Template ------------------------
void CreateButton(Sprite &sprite, Texture &texture, const string &filePath,
                  float xPosition = WindowWidth / 2.f,
                  float yPositon = WindowHeight / 2.f, float scaleX = 1.f, float scaleY = 1.f)
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
        if (auto mouseEvent = event->getIf<Event::MouseButtonReleased>())
            if (mouseEvent->button == Mouse::Button::Left)
            {
                // if The Button "NEW" is pressed and (you're on the switchUser Menu)
                if (NewButton.getGlobalBounds().contains(mousePos) and !isConfirmUserDelete and !isListFull)
                    EnterYourName();
                // Try adding the new name
                if (NameEntry and DoneAddingUser.getGlobalBounds().contains(mousePos))
                {
                    if (NumberOfUsers < MaxNumberOfUsers) // if you Can
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

                            if (DupplicateName) // YES?
                            {
                                if (!CamefromDupplicate)
                                    InputString = "";
                                DupplicateUser(); // it's dupplicated you can't add it
                            }
                            if (!DupplicateName) // NO? -> add it
                            {
                                // ofstream adduser("Assets/Switch User/Users_List.txt");
                                players[NumberOfUsers].name = InputString;
                                players[NumberOfUsers].id = NumberOfUsers + 1;
                                NumberOfUsers++;
                                SaveUsers();
                                InputString = "";
                                // for (int id = 0; id < NumberOfUsers; id++)
                                //   adduser << players[id].name << '\t' << players[id].id << '\n;
                                // adduser.close();
                                RefreshUsersList();                        // Refresh The List after each addition
                                CurUser = players[NumberOfUsers - 1].name; //[NumberOfUsers - 1] becuase we already did NumberOfUsers++
                            }
                        }
                    }
                    else if (NumberOfUsers >= MaxNumberOfUsers) // Can't add it?
                    {
                        NameEntry = 0;
                        FullList(); // the list is full (has 7 Users already)
                        if (isListFull)
                            if (mouseEvent->button == Mouse::Button::Left)
                                if (FullOKButton.getGlobalBounds().contains(mousePos)) // after pressing ok
                                    isListFull = 0;                                    // to get out of the POP-UP massege
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
                        SelectUserHL.setPosition({X, Y});
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
                        // ofstream rewrite("Assets/Switch User/Users_List.txt");
                        // for (int id = 0; id < NumberOfUsers; id++)
                        //     rewrite << players[id].name << '\t' << players[id].id << '\t' << players[id].highscore << '\n';
                        // rewrite.close();
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
        if (const auto *typed = event->getIf<Event::TextEntered>())
        {
            char32_t a = typed->unicode;
            if (a == 8 /*ASCII for BackSpace*/ and !InputString.empty())
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
        Blink.setPosition({X, Y});
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
    CreateButton(EnterYourNamebg, EnterYourNameBgTex, "Assets/Switch User/EnterYourName.png", X, Y, 0.2, 0.2);
    if (!Inputfont.openFromFile("Assets/Fonts/trebuc.ttf"))
        cout << "Cant Open Font!";
    DisplayText.setFont(Inputfont);
    DisplayText.setCharacterSize(30);
    DisplayText.setFillColor(Color::White);
    // X = WindowWidth * 0.38f, Y = WindowHeight * 0.30f;
    // DisplayText.setPosition({(float)X, (float)Y});
    X = WindowWidth * 0.24f, Y = WindowHeight * 0.34f;
    DisplayText.setPosition({(float)X, (float)Y});
    X = WindowWidth * 0.375f, Y = WindowHeight * 0.448f;
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
        float Y = WindowHeight * 0.23f + (id * 40.f);
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
    CreateButton(DeleteUserBg, DelteUserBgTex, "Assets/Switch User/DeleteUserBg.png", X, Y, 0.175, 0.175);
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
    CreateButton(ListisFull, ListisFullTex, "Assets/Switch User/ListIsFull.png", X, Y, 0.2, 0.2);
    X = WindowWidth * 0.5f, Y = WindowHeight * 0.64f;
    CreateButton(FullOKButton, FullOKButtonTex, "Assets/Switch User/Button.png", X, Y, 1.5, 1.5);
    SetupButtonText(FullListOKBtnText, "OK", FullOKButton);
    InputString = "";
}
void DupplicateUser()
{
    DupplicateName = 1;
    unsigned int X = WindowWidth * 0.5f, Y = WindowHeight * 0.5f;
    CreateButton(DupplicateBg, DupplicateBgTex, "Assets/Switch User/DupplicateUser.png", X, Y, 0.2, 0.2);
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
    // Redraw the MainMenu Background
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
        // Get Back to the Main Menu
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

// [ADDED] Full OptionsMenu wrapper function (placed after DisplaySwitchUser)
void OptionsMenu()
{
    shouldCloseOptions = false; // تصفير المتغير
    StartOptions();
    Clock clock;
    while (window.isOpen())
    {
        float dt1 = clock.restart().asSeconds();
        totaltime += dt1;

        BarracudaFishanimation();
        QueenTriggerFish();
        for (auto &obj : smallfishs)
        {
            obj.shape.setPosition({obj.sprite.getPosition().x, obj.sprite.getPosition().y});
            obj.update(286, 126);

            float posX = obj.sprite.getPosition().x;
            if (posX <= -150.f || posX >= WindowWidth + 150.f)
            {
                obj.velocityX_AXIS *= -1;
                // obj.changedir *= -1;
                // obj.sprite.setScale({0.2f * obj.changedir, 0.2f});
            }
        }

        float dt = clock.restart().asSeconds();
        while (const optional event = window.pollEvent())
        {
            if (event->is<Event::Closed>())
                window.close();
            else if (const auto *resizeEvent = event->getIf<Event::Resized>())
            {
                // [تعديل] هنا المشكلة التانية كانت بتحصل
                // view.setSize(Vector2f(resizeEvent->size)); // <-- امسح السطر ده خالص أو علقه
                view.setViewport(FloatRect({0.f, 0.f}, {1.f, 1.f})); // خليه يملأ الشاشة
                window.setView(view);
            }
        }

        // UpdateMainMenuFish(); // تحريك السمك

        UpdateOptions(); // تشغيل المنطق

        // [تعديل] الخروج من اللوب لو تم الضغط على Done
        if (shouldCloseOptions)
            return; // يرجع لـ MainMenu اللي كانت ناداه

        DrawOptions();
    }
}

// =====================================================================
// SECTION 4: HELPER UTILITY FUNCTIONS
// Reusable code blocks for aligning text and drawing sprites cleanly.
// =====================================================================
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

    // تأكد إنك مش بتعيد حساب الـ Origin بناء على WindowWidth القديم
    // الـ Origin يكون نص حجم التكستشر نفسها
    auto textureSize = sf::Vector2f(texture.getSize());
    sprite.setOrigin(textureSize / 2.f);

    window.draw(sprite);
}
// =====================================================================
// SECTION 5: INITIALIZATION (START)
// Loads all files from the hard drive and positions the UI elements.
// =====================================================================
void StartOptions()
{
    window.setFramerateLimit(60);
    window.setView(view);
    srand(static_cast<unsigned int>(time(NULL)));
    if (!globalFont.openFromFile("Assets/Fonts/Barmeno.ttf"))
        cout << "Failed to load font!" << endl; // [MODIFIED] Corrected path based on context
    globalFont.setSmooth(false);
    screenDarkener.setSize(Vector2f(WindowWidth, WindowHeight));
    screenDarkener.setFillColor(Color(0, 0, 0, 95));
    // [MODIFIED] We don't need main_bg image anymore as requested

    if (texBgPlank.loadFromFile("Assets/Options/plank.png"))
    { // [MODIFIED] Corrected path
        texBgPlank.setSmooth(true);
        sprBgPlank.emplace(texBgPlank);
        FloatRect b = sprBgPlank->getLocalBounds();
        sprBgPlank->setOrigin({b.size.x / 2.f, b.size.y / 2.f});
        sprBgPlank->setPosition({WindowWidth / 2.f, 280.f});
    }
    if (texCorals.loadFromFile("Assets/Options/corals.png"))
    { // [MODIFIED] Corrected path
        texCorals.setSmooth(true);
        sprCorals.emplace(texCorals);
        FloatRect b = sprCorals->getLocalBounds();
        sprCorals->setOrigin({b.size.x / 2.f, b.size.y / 2.f});
        sprCorals->setPosition({(WindowWidth - 2) / 2.f, 315.f});
        sprCorals->setColor(Color(255, 255, 255, 175));
    }
    if (texTitle.loadFromFile("Assets/Options/options_title.png"))
    { // [MODIFIED] Corrected path
        texTitle.setSmooth(true);
        sprTitle.emplace(texTitle);
        FloatRect b = sprTitle->getLocalBounds();
        sprTitle->setOrigin({b.size.x / 2.f, b.size.y / 2.f});
        sprTitle->setPosition({WindowWidth / 2.f, 100.f});
        sprTitle->setScale({1.f, 1.f});
    }
    if (WaterSound.loadFromFile("Assets/Music and Sounds/waterloop1.ogg"))
    { // [MODIFIED] Corrected path
        waterloop.emplace(WaterSound);
        waterloop->setLooping(true);
    }
    (void)sfxBuffers[0].loadFromFile("Assets/Music and Sounds/wateramb1.ogg"); // [MODIFIED] Corrected path
    (void)sfxBuffers[1].loadFromFile("Assets/Music and Sounds/wateramb2.ogg"); // [MODIFIED] Corrected path
    (void)sfxBuffers[2].loadFromFile("Assets/Music and Sounds/wave1.ogg");     // [MODIFIED] Corrected path
    (void)sfxBuffers[3].loadFromFile("Assets/Music and Sounds/wave2.ogg");     // [MODIFIED] Corrected path
    nextSfxDelay = (rand() % 2500 + 2500) / 1000.f;
    (void)texUncheckedNormal.loadFromFile("Assets/Options/_shell_checkbtn_normal.png");     // [MODIFIED] Corrected path
    (void)texUncheckedHover.loadFromFile("Assets/Options/_shell_checkbtn_high.png");        // [MODIFIED] Corrected path
    (void)texCheckedNormal.loadFromFile("Assets/Options/shell_checkbtnchecked_normal.png"); // [MODIFIED] Corrected path
    (void)texCheckedHover.loadFromFile("Assets/Options/_shell_checkbtnchecked_high.png");   // [MODIFIED] Corrected path
    (void)texDoneNormal.loadFromFile("Assets/Options/done_normal.png");                     // [MODIFIED] Corrected path
    (void)texDoneHover.loadFromFile("Assets/Options/done_hover.png");                       // [MODIFIED] Corrected path
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
    versionLabel->setPosition({170.f, 423.f});
    versionLabel->setFillColor(Color(200, 210, 220));
    const char *labels[] = {"Sound:", "Music:", "Full Screen:", "Slow", "", "", "Fast", "Low", "", "High", "Done"};
    for (int i = 0; i < NumOptions; i++)
    {
        OptionButtons[i].text.emplace(globalFont);
        OptionButtons[i].text->setString(labels[i]);
        OptionButtons[i].isCheckbox = true;
        OptionButtons[i].isHovered = false;
        OptionButtons[i].checkbox.emplace(texUncheckedNormal);
        if (i < 3)
        {
            // أول خيارين (Sound و Music) يبقوا شغالين، والتالت (Full Screen) يبقى شغال برضو
            if (i == 2)
                OptionButtons[i].isChecked = true; // عشان اشتغلنا Full Screen
            else
                OptionButtons[i].isChecked = true; // الصوت والموسيقى            OptionButtons[i].text->setCharacterSize(22);
            OptionButtons[i].text->setFillColor(paleText);
            float yPos = POS_Y_TOP_OPTIONS + (i * LAYOUT_ROW_SPACING);
            setRightAligned(OptionButtons[i].text, LAYOUT_LABEL_X, yPos);
            OptionButtons[i].checkbox->setPosition({LAYOUT_BUBBLE_START_X, yPos + 7.f});
        }
        else if (i >= 3 && i <= 6)
        {
            OptionButtons[i].isChecked = (i == 3);
            OptionButtons[i].text->setCharacterSize(14);
            OptionButtons[i].text->setFillColor(paleText);
            float startX = LAYOUT_BUBBLE_START_X + ((i - 3) * LAYOUT_COL_SPACING);
            OptionButtons[i].checkbox->setPosition({startX, POS_Y_MOUSE_SPEED + 7.f});
            setTopCentered(OptionButtons[i].text, startX, POS_Y_MOUSE_SPEED + 20.f);
        }
        else if (i >= 7 && i <= 9)
        {
            OptionButtons[i].isChecked = (i == 9);
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
            OptionButtons[i].text->setCharacterSize(26);
            OptionButtons[i].text->setFillColor(Color(180, 255, 50));
            OptionButtons[i].text->setOutlineColor(Color(80, 40, 0));
            OptionButtons[i].text->setOutlineThickness(3.f);
            FloatRect textBounds = OptionButtons[i].text->getLocalBounds();
            OptionButtons[i].text->setOrigin({textBounds.size.x / 2.f, textBounds.size.y / 2.f});
            OptionButtons[i].text->setPosition({WindowWidth / 2.f, 445.f});
        }
    }
}
// =====================================================================
// SECTION 6: GAME LOGIC & INPUT (UPDATE)
// Handles mouse positioning, hit detection, hovering, and clicking.
// =====================================================================
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
                // [تعديل] عالجنا الـ Fullscreen لوحده عشان نضمن إنه بيملأ الشاشة
                else if (i == 2)
                {
                    OptionButtons[i].isChecked = !OptionButtons[i].isChecked;

                    if (OptionButtons[i].isChecked)
                        window.create(VideoMode::getDesktopMode(), "Feeding Frenzy 2", State::Fullscreen);
                    else
                        window.create(VideoMode({800, 600}), "Feeding Frenzy 2", State::Windowed);

                    // ضبط الكاميرا للملء الكامل (مفيش كود نسب هنا)
                    view.setSize({800.f, 600.f});
                    view.setCenter({400.f, 300.f});
                    view.setViewport(FloatRect({0.f, 0.f}, {1.f, 1.f}));

                    window.setView(view);
                    window.setFramerateLimit(60);
                }
                // [تعديل] باقي الأزرار (Sound و Music)
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

        // تغيير لون التيكست
        if (!OptionButtons[i].isCheckbox && OptionButtons[i].text)
        {
            if (OptionButtons[i].isHovered)
                OptionButtons[i].text->setFillColor(Color(240, 210, 0));
            else
                OptionButtons[i].text->setFillColor(Color(180, 255, 50));
        }
    }

    // تشغيل الصوت
    if (OptionButtons[0].isChecked)
    {
        if (waterloop && waterloop->getStatus() != Sound::Status::Playing)
            waterloop->play();
        // ... (كود الـ SFX لو عايز تضيفه) ...
    }
    else
    {
        if (waterloop)
            waterloop->stop();
        for (int c = 0; c < NUM_SOUND_CHANNELS; c++)
            if (sfxChannels[c])
                sfxChannels[c]->stop();
    }

    // تشغيل الموسيقى
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
// =====================================================================
// SECTION 7: RENDERING (DRAW)
// Clears the screen, draws all background layers, UI elements, and displays.
// =====================================================================
void DrawOptions()
{
    window.setView(view);
    // [MODIFIED] Removed window.clear(Color::Black); and sprBgMain drawing,
    // instead we draw the main menu background to keep fishes visible.
    // window.clear(Color::Black);
    // if (sprBgMain) window.draw(*sprBgMain);
    DrawMainMenuBackground(); // [ADDED]

    window.draw(screenDarkener);
    // --- UPDATED: The Bulletproof 4-Way Outline Trick ---
    if (sprBgPlank)
    {
        Sprite plankOutline = *sprBgPlank;
        // --- ALPHA LOWERED TO 90 for a softer, more transparent shadow! ---
        plankOutline.setColor(Color(0, 0, 0, 90));
        Vector2f pos = sprBgPlank->getPosition();
        float outlineThickness = 3.f; // The thickness of the shadow edge
        // Draw the soft shadow shifted in 4 directions
        plankOutline.setPosition({pos.x - outlineThickness, pos.y});
        window.draw(plankOutline);
        plankOutline.setPosition({pos.x + outlineThickness, pos.y});
        window.draw(plankOutline);
        plankOutline.setPosition({pos.x, pos.y - outlineThickness});
        window.draw(plankOutline);
        plankOutline.setPosition({pos.x, pos.y + outlineThickness});
        window.draw(plankOutline);
        // Restore original position and draw the colorful plank on top
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

// [ADDED] Helper to update fish positions in the background of the options menu
void UpdateMainMenuFish()
{
    GreenfishAnimation();
    MinowFishanimation();
    BarracudaFishanimation();
    QueenTriggerFish();
    for (auto &obj : smallfishs)
    {
        obj.shape.setPosition({obj.sprite.getPosition().x, obj.sprite.getPosition().y});
        obj.update(286, 126);
        if (obj.sprite.getPosition().x <= 0 || obj.sprite.getPosition().x >= WindowWidth)
        {
            obj.velocityX_AXIS *= -1;
            // obj.changedir *= -1;
            // obj.sprite.setScale({0.2f * obj.changedir, 0.2f});
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
        for (auto &obj : smallfishs)
        {
            obj.shape.setPosition({obj.sprite.getPosition().x, obj.sprite.getPosition().y});
            obj.update(286, 126);

            float posX = obj.sprite.getPosition().x;
            if (posX <= -150.f || posX >= WindowWidth + 150.f)
            {
                obj.velocityX_AXIS *= -1;
                // obj.changedir *= -1;
                // obj.sprite.setScale({0.2f * obj.changedir, 0.2f});
            }
        }

        while (const optional event = window.pollEvent())
        {
            if (event->is<Event::Closed>())
            {
                window.close();
                return;
            }

            // [تعديل] هنا بنستقبل حدث رفع زرار الماوس
            if (auto mouseEvent = event->getIf<Event::MouseButtonReleased>())
            {
                if (mouseEvent->button == Mouse::Button::Left)
                {
                    Vector2f mousePos = window.mapPixelToCoords(Mouse::getPosition(window), view);

                    // لازم نحدد مكان الأزرار عشان نفحص الـ Bounds
                    Vector2f yesPos = {WindowWidth / 2.f - 100.f, WindowHeight / 2.f + 90.f};
                    Vector2f noPos = {WindowWidth / 2.f + 100.f, WindowHeight / 2.f + 90.f};

                    // فحص زرار Yes
                    quit_button_sprite.setPosition(yesPos);
                    if (quit_button_sprite.getGlobalBounds().contains(mousePos))
                    {
                        window.close(); // اقفل اللعبة
                        return;
                    }

                    // فحص زرار No
                    quit_button_sprite.setPosition(noPos);
                    if (quit_button_sprite.getGlobalBounds().contains(mousePos))
                    {
                        return; // ارجع للمينيو (الـ Event اتستهلك هنا وممش هيرح للـ Menu)
                    }
                }
            }
        }

        window.setView(view);

        UpdateQuit(); // بس خليها للرسم والـ Hover بس

        DrawQuit();
    }
}

void StartQuit()
{
    // --- [1] تحميل الخط ---
    if (!btnFont.openFromFile("Assets/Fonts/BarmenoBold.otf"))
    {
        cout << "Error: Could not load font!" << endl;
    }

    // --- [2] تحميل وتجهيز الخلفية ---
    if (!quit_bg.loadFromFile("Assets/Quit/bg_quit.png"))
        cout << "Error Loading Quit BG!" << endl;

    quit_bg_sprite.setTexture(quit_bg);
    quit_bg.setSmooth(true);

    // تحديد حجم الخلفية
    float targetWidth = 500.f;
    float targetHeight = 300.f;
    Vector2u texture_Size = quit_bg.getSize();
    float ScaleX = targetWidth / texture_Size.x;
    float ScaleY = targetHeight / texture_Size.y;

    quit_bg_sprite.setOrigin({texture_Size.x / 2.f, texture_Size.y / 2.f});
    quit_bg_sprite.setPosition({WindowWidth / 2.f, WindowHeight / 2.f});
    quit_bg_sprite.setScale({ScaleX, ScaleY});

    // --- [3] تجهيز الأيقونة ---
    quit_icon_sprite.setOrigin({quit_icon.getSize().x / 2.f, quit_icon.getSize().y / 2.f});
    quit_icon_sprite.setPosition({WindowWidth / 2.f, WindowHeight / 2.f - 133.f});
    quit_icon_sprite.setScale({0.25f, 0.25f});

    // --- [4] تجهيز الأزرار (Sprites) ---
    quit_button.setSmooth(true);
    quit_button_hover.setSmooth(true);
    quit_button_sprite.setTexture(quit_button);
    quit_button_hover_sprite.setTexture(quit_button_hover);

    // نوسط السبرايت (الزرار نفسه)
    Vector2f btnSize = {(float)quit_button.getSize().x, (float)quit_button.getSize().y};
    quit_button_sprite.setOrigin({btnSize.x / 2.f, btnSize.y / 2.f});
    quit_button_hover_sprite.setOrigin({btnSize.x / 2.f, btnSize.y / 2.f});

    // --- [5] تجهيز النصوص (Texts) ---

    // 1. نص السؤال (Title)
    quit_title_text.setFont(btnFont);
    quit_title_text.setString("Are you sure you want to quit");
    quit_title_text.setCharacterSize(28);
    quit_title_text.setFillColor(Color(255, 255, 255)); // ابيض
    quit_title_text.setOutlineThickness(2);
    quit_title_text.setOutlineColor(Color(80, 40, 0));
    FloatRect titleBounds = quit_title_text.getLocalBounds();
    // معادلة التوسيط الصح في SFML 3
    quit_title_text.setOrigin({titleBounds.position.x + titleBounds.size.x / 2.f, titleBounds.position.y + titleBounds.size.y / 2.f});
    quit_title_text.setPosition({WindowWidth / 2.f, WindowHeight / 2.f - 30.f});

    quit_title_text2.setFont(btnFont);
    quit_title_text2.setString("Feeding Frenzy 2?");
    quit_title_text2.setCharacterSize(28);
    quit_title_text2.setFillColor(Color(255, 255, 255)); // ابيض
    quit_title_text2.setOutlineThickness(2);
    quit_title_text2.setOutlineColor(Color(80, 40, 0));
    FloatRect titleBounds2 = quit_title_text2.getLocalBounds();
    // معادلة التوسيط الصح في SFML 3
    quit_title_text2.setOrigin({titleBounds2.position.x + titleBounds2.size.x / 2.f, titleBounds2.position.y + titleBounds2.size.y / 2.f});
    quit_title_text2.setPosition({WindowWidth / 2.f, WindowHeight / 2.f + 10.f});

    // 2. زرار Yes
    quit_yes_text.setFont(btnFont);
    quit_yes_text.setString("Yes");
    quit_yes_text.setCharacterSize(24);
    // اللون الأخضر الفاتح المطلوب
    quit_yes_text.setFillColor(Color(180, 255, 100));
    quit_yes_text.setOutlineThickness(2);
    quit_yes_text.setOutlineColor(Color(20, 60, 0));

    FloatRect yesBounds = quit_yes_text.getLocalBounds();
    // معادلة التوسيط عشان يبقى في نص الزرار
    quit_yes_text.setOrigin({yesBounds.position.x + yesBounds.size.x / 2.f, yesBounds.position.y + yesBounds.size.y / 2.f});

    // مكان الزرار (نزلت قيمة Y عشان ينزلوا تحت)
    quit_yes_text.setPosition({WindowWidth / 2.f - 100.f, WindowHeight / 2.f + 90.f});

    // 3. زرار No
    quit_no_text.setFont(btnFont);
    quit_no_text.setString("No!");
    quit_no_text.setCharacterSize(24);
    quit_no_text.setFillColor(Color(180, 255, 100)); // نفس اللون الأخضر
    quit_no_text.setOutlineThickness(2);
    quit_no_text.setOutlineColor(Color(20, 60, 0));

    FloatRect noBounds = quit_no_text.getLocalBounds();
    quit_no_text.setOrigin({noBounds.position.x + noBounds.size.x / 2.f, noBounds.position.y + noBounds.size.y / 2.f});

    // مكان الزرار (نفس المستوى بتاع Yes)
    quit_no_text.setPosition({WindowWidth / 2.f + 100.f, WindowHeight / 2.f + 90.f});

    // تصفير الفلات
    quit_yes_pressed = false;
    quit_no_pressed = false;
}
void UpdateQuit()
{
    PlayingSound(false);
    Vector2f mousePos = window.mapPixelToCoords(Mouse::getPosition(window), view);

    Vector2f yesPos = {WindowWidth / 2.f - 100.f, WindowHeight / 2.f + 90.f};
    Vector2f noPos = {WindowWidth / 2.f + 100.f, WindowHeight / 2.f + 90.f};

    // --- منطق Hover لزرار Yes ---
    quit_button_sprite.setPosition(yesPos);
    FloatRect yesBounds = quit_button_sprite.getGlobalBounds();

    if (yesBounds.contains(mousePos))
    {
        quit_yes_text.setFillColor(Color(255, 255, 0)); // أصفر
    }
    else
    {
        quit_yes_text.setFillColor(Color(180, 255, 100)); // أخضر
    }

    // --- منطق Hover لزرار No ---
    quit_button_sprite.setPosition(noPos);
    FloatRect noBounds = quit_button_sprite.getGlobalBounds();

    if (noBounds.contains(mousePos))
    {
        quit_no_text.setFillColor(Color(255, 255, 0)); // أصفر
    }
    else
    {
        quit_no_text.setFillColor(Color(180, 255, 100)); // أخضر
    }
}

void DrawQuit()
{
    // 1. خلفية المينيو
    DrawMainMenuBackground();

    // 2. خلفية شاشة الخروج
    window.draw(quit_bg_sprite);

    // 3. الأيقونة
    window.draw(quit_icon_sprite);

    // 4. نص السؤال
    window.draw(quit_title_text);

    window.draw(quit_title_text2);

    // 5. زرار Yes
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

    // 6. زرار No
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

// دالة للتلاشي للأسود (Fade Out) - بتستخدم لما تخرج من شاشة
void FadeOutToBlack()
{
    fadeAlpha = 0.f;
    fadeClock.restart();

    while (fadeAlpha < 255.f)
    {
        fadeAlpha = std::min(255.f, fadeClock.getElapsedTime().asSeconds() * 800.f);
        fadeRect.setFillColor(Color(0, 0, 0, (uint8_t)fadeAlpha));

        window.setView(view);
        // رسم المينيو وهو بيختفي
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

// دالة للظهور من الأسود (Fade In) - بتستخدم لما تدخل شاشة جديدة
void FadeInFromBlack()
{
    fadeAlpha = 255.f;
    fadeClock.restart();

    while (fadeAlpha > 0.f)
    {
        fadeAlpha = std::max(0.f, 255.f - fadeClock.getElapsedTime().asSeconds() * 800.f);
        fadeRect.setFillColor(Color(0, 0, 0, (uint8_t)fadeAlpha));

        window.setView(view);
        // رسم المينيو وهو بيظهر
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
            f1 << story_scores[i].name << "\n"
               << story_scores[i].score << "\n";

    ofstream f2("highscore_timeattack.txt");
    if (f2.is_open())
        for (int i = 0; i < MAX_SCORES; i++)
            f2 << timeattack_scores[i].name << "\n"
               << timeattack_scores[i].score << "\n";
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

    // 1. تحميل البيانات من الملفات
    loadFile("highscore_story.txt", story_scores, "Mr. Minnow");
    loadFile("highscore_timeattack.txt", timeattack_scores, "Speedy");

    // [تعديل] 2. ترتيب البيانات تنازلياً فور تحميلها
    proceduralSort(story_scores);
    proceduralSort(timeattack_scores);
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
    string dName = (currentMode == STORY) ? "Mr. Minnow" : "Speedy";
    HighScoreEntry *activeArr = (currentMode == STORY) ? story_scores : timeattack_scores;
    for (int i = 0; i < MAX_SCORES; i++)
        activeArr[i] = {dName, (MAX_SCORES - i) * 5000};
    scrollOffset = 0;
    updateHighScoreTexts();
    saveAllScores();
}

// ================= WRAPPER =================
void Highscore()
{
    StartHighscore();

    // --- Fade In (ظهور من الأسود) ---
    fadeAlpha = 255.f;
    fadeClock.restart();
    while (fadeAlpha > 0.f)
    {
        fadeAlpha = std::max(0.f, 255.f - fadeClock.getElapsedTime().asSeconds() * 800.f);
        fadeRect.setFillColor(Color(0, 0, 0, (uint8_t)fadeAlpha));

        window.setView(view);

        // هنرسم محتوى الهاي سكور يدوياً هنا عشان متعملش Display مرتين
        DrawMainMenuBackground(); // دي بتعمل Clear

        // رسم كل عناصر الهاي سكور
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

        // رسم الشاشة السودا الشفافة
        window.draw(fadeRect);
        window.display(); // Display واحدة بس في الآخر
    }

    // --- اللوب الأساسي ---
    Clock frameClock;
    while (window.isOpen())
    {
        float dt = frameClock.restart().asSeconds();
        totaltime += dt;

        BarracudaFishanimation();
        QueenTriggerFish();
        for (auto &obj : smallfishs)
        {
            obj.shape.setPosition({obj.sprite.getPosition().x, obj.sprite.getPosition().y});
            obj.update(286, 126);

            float posX = obj.sprite.getPosition().x;
            if (posX <= -150.f || posX >= WindowWidth + 150.f)
            {
                obj.velocityX_AXIS *= -1;
                // obj.changedir *= -1;
                // obj.sprite.setScale({0.2f * obj.changedir, 0.2f});
            }
        }

        UpdateHighscore();
        DrawHighscore(); // هنا بترجع تعمل Display براحتها
    }

    // مفيش Fade Out هنا، عشان MainMenu بتعمله
}

void addNewHighScore(string name, int score, bool isStoryMode)
{
    // 1. نحدد هيشتغل على أي قائمة (Story ولا Time Attack)
    HighScoreEntry *activeList = isStoryMode ? story_scores : timeattack_scores;

    // 2. [خطوة ذكية] نتأكد إن السكور الجديد يستحق الدخول
    // لو السكور الجديد أقل أو يساوي أقل سكور موجود (اللي في الآخر)، يبقى مش هندخله عشان نحافظ على القائمة القوية
    if (score < activeList[MAX_SCORES - 1].score)
    {
        return; // السكور ضعيف، مش هيدخل
    }

    // 3. نستبدل آخر عنصر (صاحب أقل سكور) بالسكور الجديد
    activeList[MAX_SCORES - 1].name = name;
    activeList[MAX_SCORES - 1].score = score;

    // 4. نرتب القائمة فوراً
    // لو السكور عالي هيطلع للأول، ولو متوسط هيبقى في النص
    proceduralSort(activeList);

    // 5. نحفظ التعديلات في ملفات اللعبة
    saveAllScores();

    // 6. نحدث النصوص اللي على الشاشة
    updateHighScoreTexts();
}

// ================= START =================
void StartHighscore()
{
    window.setFramerateLimit(60);
    window.setView(view);

    // Smooth
    // texHSBackground.setSmooth(true);
    texHSDoneNormal.setSmooth(true);
    texHSDoneHover.setSmooth(true);
    texHSResetNormal.setSmooth(true);
    texHSResetHover.setSmooth(true);
    texHSArrowNormal.setSmooth(true);
    texHSArrowHover.setSmooth(true);
    texHSArrowDown.setSmooth(true);

    // Background
    // // sprHSBackground.setTexture(texHSBackground);
    // sprHSBackground.setPosition({ 0.f, 0.f });
    // sprHSBackground.setScale({
    //     WindowWidth  / (float)texHSBackground.getSize().x,
    //     WindowHeight / (float)texHSBackground.getSize().y
    // });

    // Done Button
    sprHSDonePlank.setTexture(texHSDoneNormal);
    sprHSDonePlank.setOrigin({sprHSDonePlank.getLocalBounds().size.x / 2.f,
                              sprHSDonePlank.getLocalBounds().size.y / 2.f});
    sprHSDonePlank.setPosition({400.f, 470.f});

    // Reset Button
    sprHSResetPlank.setTexture(texHSResetNormal);
    sprHSResetPlank.setOrigin({sprHSResetPlank.getLocalBounds().size.x / 2.f,
                               sprHSResetPlank.getLocalBounds().size.y / 2.f});
    sprHSResetPlank.setPosition({400.f, 560.f});

    // Arrows
    sprHSStoryArrowRight.setTexture(texHSArrowNormal);
    sprHSStoryArrowRight.setOrigin({texHSArrowNormal.getSize().x / 2.f, texHSArrowNormal.getSize().y / 2.f});
    sprHSStoryArrowRight.setPosition({480.f, 130.f});

    sprHSStoryArrowLeft.setTexture(texHSArrowNormal);
    sprHSStoryArrowLeft.setOrigin({texHSArrowNormal.getSize().x / 2.f, texHSArrowNormal.getSize().y / 2.f});
    sprHSStoryArrowLeft.setPosition({320.f, 130.f});
    sprHSStoryArrowLeft.setScale({-1.f, 1.f});

    sprHSListArrowUp.setTexture(texHSArrowNormal);
    sprHSListArrowUp.setOrigin({texHSArrowNormal.getSize().x / 2.f, texHSArrowNormal.getSize().y / 2.f});
    sprHSListArrowUp.setPosition({570.f, LIST_START_Y + 10.f});
    sprHSListArrowUp.setRotation(degrees(270.f));

    sprHSListArrowDown.setTexture(texHSArrowNormal);
    sprHSListArrowDown.setOrigin({texHSArrowNormal.getSize().x / 2.f, texHSArrowNormal.getSize().y / 2.f});
    sprHSListArrowDown.setPosition({570.f, LIST_START_Y + (9 * LIST_SPACING) + 10.f});
    sprHSListArrowDown.setRotation(degrees(90.f));

    // Main Title
    textHSMainTitle.setFillColor(colorTitleGreen);
    textHSMainTitle.setOutlineThickness(2.f);
    textHSMainTitle.setOrigin({textHSMainTitle.getLocalBounds().size.x / 2.f, 0.f});
    textHSMainTitle.setPosition({400.f, 40.f});

    // Story Mode Label
    textHSStoryMode.setOrigin({textHSStoryMode.getLocalBounds().size.x / 2.f, 0.f});
    textHSStoryMode.setPosition({400.f, 115.f});

    // Underline
    HSModeUnderline.setFillColor(Color::White);
    HSModeUnderline.setSize({textHSStoryMode.getLocalBounds().size.x, 2.f});
    HSModeUnderline.setOrigin({HSModeUnderline.getSize().x / 2.f, 0.f});
    HSModeUnderline.setPosition({400.f, 142.f});

    // List Texts
    for (int i = 0; i < VISIBLE_SCORES; i++)
    {
        textHSListRanks[i].setFillColor(colorListText);
        textHSListNames[i].setFillColor(colorListText);
        textHSListScores[i].setFillColor(colorListText);

        textHSListRanks[i].setPosition({260.f, LIST_START_Y + (i * LIST_SPACING)});
        textHSListNames[i].setPosition({290.f, LIST_START_Y + (i * LIST_SPACING)});
    }

    // Done Button Text
    textHSDoneButton.setFillColor(colorDoneText);
    textHSDoneButton.setOrigin({textHSDoneButton.getLocalBounds().size.x / 2.f,
                                textHSDoneButton.getLocalBounds().size.y / 2.f});
    textHSDoneButton.setOutlineThickness(2.f);
    textHSDoneButton.setPosition({400.f, 461.f});

    // Reset Button Text
    textHSResetButton.setFillColor(colorResetText);
    textHSResetButton.setOrigin({textHSResetButton.getLocalBounds().size.x / 2.f,
                                 textHSResetButton.getLocalBounds().size.y / 2.f});
    textHSResetButton.setPosition({400.f, 557.f});

    loadAllScores();
    updateHighScoreTexts();
}

// ================= UPDATE =================
void UpdateHighscore()
{
    PlayingSound(false);
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

    // Lambda للـ Arrows (بدون pointers)
    auto processArrow = [&](Sprite &s, bool action)
    {
        if (s.getGlobalBounds().contains(mousePos))
        {
            if (isMousePressed)
            {
                s.setTexture(texHSArrowDown);
                s.setOrigin({texHSArrowDown.getSize().x / 2.f, texHSArrowDown.getSize().y / 2.f});
            }
            else
            {
                s.setTexture(texHSArrowHover);
                s.setOrigin({texHSArrowHover.getSize().x / 2.f, texHSArrowHover.getSize().y / 2.f});
            }
            return action;
        }
        else
        {
            s.setTexture(texHSArrowNormal);
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

    // Done Button
    if (sprHSDonePlank.getGlobalBounds().contains(mousePos))
    {
        sprHSDonePlank.setTexture(texHSDoneHover);
        textHSDoneButton.setFillColor(colorHoverHighlight);
        if (mouseClicked)
        {
            window.setView(view); // ضروري عشان الـ Mouse Position يكون صح في كل الشاشات
            MainMenu();
        } // رجوع للـ MainMenu
    }
    else
    {
        sprHSDonePlank.setTexture(texHSDoneNormal);
        textHSDoneButton.setFillColor(colorDoneText);
    }

    // Reset Button
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

// ================= DRAW =================
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

// ==========================================
// 1. الدالة الرئيسية (Wrapper)
// بتنظم عملية الـ Fade In/Out واللوب الأساسي
// ==========================================

void Credits()
{
    StartCredits();

    // --- 1. Fade In (ظهور من الأسود) ---
    fadeAlpha = 255.f;
    fadeClock.restart();
    while (fadeAlpha > 0.f)
    {
        fadeAlpha = std::max(0.f, 255.f - fadeClock.getElapsedTime().asSeconds() * 800.f);
        fadeRect.setFillColor(Color(0, 0, 0, (uint8_t)fadeAlpha));

        window.setView(view);
        DrawCredits();         // رسم العناصر
        window.draw(fadeRect); // رسم الشاشة السودا
        window.display();
    }
    Clock frameClock;
    // --- 2. اللوب الأساسي ---
    while (window.isOpen())
    {
        float dt = frameClock.restart().asSeconds();
        totaltime += dt;

        BarracudaFishanimation();
        QueenTriggerFish();
        for (auto &obj : smallfishs)
        {
            obj.shape.setPosition({obj.sprite.getPosition().x, obj.sprite.getPosition().y});
            obj.update(286, 126);

            float posX = obj.sprite.getPosition().x;
            if (posX <= -150.f || posX >= WindowWidth + 150.f)
            {
                obj.velocityX_AXIS *= -1;
                // obj.changedir *= -1;
                // obj.sprite.setScale({0.2f * obj.changedir, 0.2f});
            }
        }
        // لو Update رجعت true، يبقى المستخدم عايز يخرج
        if (UpdateCredits())
            break;

        window.setView(view);
        DrawCredits(); // رسم العناصر
        window.display();
    }

    // --- 3. Fade Out (تلاشي للأسود) ---
    fadeAlpha = 0.f;
    fadeClock.restart();
    while (fadeAlpha < 255.f)
    {
        fadeAlpha = std::min(255.f, fadeClock.getElapsedTime().asSeconds() * 800.f);
        fadeRect.setFillColor(Color(0, 0, 0, (uint8_t)fadeAlpha));

        window.setView(view);
        DrawCredits();         // رسم العناصر
        window.draw(fadeRect); // رسم الشاشة السودا
        window.display();
    }
}

// ==========================================
// 2. دالة التهيئة (Start)
// بتحمل الخطوط وتظبط مكان الأزرار والنصوص
// ==========================================
void StartCredits()
{
    window.setFramerateLimit(60);
    window.setView(view);

    // تهيئة النص الأساسي
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

    // تجهيز زرار Done
    sprHSDonePlankcredits.setPosition({400.f, 545.f});
    sprHSDonePlankcredits.setOrigin({texHSDoneNormalcredits.getSize().x / 2.f, texHSDoneNormalcredits.getSize().y / 2.f});

    // تجهيز النص بتاع الزرار
    textHSDoneButtoncredits.setPosition({400.f, 536.f});
    textHSDoneButtoncredits.setOutlineThickness(2.f);
    FloatRect doneBounds = textHSDoneButtoncredits.getLocalBounds();
    textHSDoneButtoncredits.setOrigin({doneBounds.size.x / 2.f, doneBounds.size.y / 2.f});
}

// ==========================================
// 3. دالة التحديث (Update)
// بتعالج الأحداث (Events) وتحسب الـ Hover
// وبترجع true لو المستخدم عايز يخرج من الشاشة
// ==========================================
bool UpdateCredits()
{
    PlayingSound(false);
    Vector2f mousePos = window.mapPixelToCoords(Mouse::getPosition(window), view);

    // 1. معالجة الأحداث (Events)
    while (const optional event = window.pollEvent())
    {
        if (event->is<Event::Closed>())
        {
            window.close();
            return true; // خروج
        }

        if (const auto *keyPressed = event->getIf<Event::KeyPressed>())
        {
            if (keyPressed->code == Keyboard::Key::Escape)
            {
                return true; // خروج
            }
        }

        if (const auto *mouseBtn = event->getIf<Event::MouseButtonReleased>())
        {
            if (mouseBtn->button == Mouse::Button::Left)
            {
                if (sprHSDonePlankcredits.getGlobalBounds().contains(mousePos))
                {
                    return true; // خروج
                }
            }
        }
    }

    // 2. تحديث الـ Hover (تغيير اللون والصورة)
    if (sprHSDonePlankcredits.getGlobalBounds().contains(mousePos))
    {
        sprHSDonePlankcredits.setTexture(texHSDoneHovercredits);
        textHSDoneButtoncredits.setFillColor(Color(255, 255, 0)); // لون أخضر فاتح
    }
    else
    {
        sprHSDonePlankcredits.setTexture(texHSDoneNormalcredits);
        textHSDoneButtoncredits.setFillColor(Color(180, 255, 100)); // لون اصفر
    }

    return false; // مفيش خروج، استمر في اللوب
}

// ==========================================
// 4. دالة الرسم (Draw)
// برسم العناصر بس من غير ما تعمل Display
// عشان نقدر نرسم الـ Fade فوقها
// ==========================================
void DrawCredits()
{
    DrawMainMenuBackground();
    if (creditsText.has_value())
        window.draw(*creditsText);
    window.draw(sprHSDonePlankcredits);
    window.draw(textHSDoneButtoncredits);
}


// ==========================================
// ===== GameScreen - حلقة الأحداث =====
// ==========================================
void GameScreen(int level)
{
    StartGameScreen(level);   // تهيئة كل عناصر شاشة اللعبة
    Clock clock;              // ساعة لحساب deltaTime

    while (window.isOpen())
    {
        float dt = clock.restart().asSeconds(); // الوقت منذ آخر frame
        totaltime += dt;                        // تحديث الوقت الكلي

        while (const optional event = window.pollEvent())
        {
            // إغلاق النافذة
            if (event->is<Event::Closed>())
                window.close();

            // تحديث الـ viewport عند تغيير حجم الشاشة
            else if (const auto* resizeEvent = event->getIf<Event::Resized>())
            {
                view.setViewport(FloatRect({ 0.f, 0.f }, { 1.f, 1.f }));
                window.setView(view);
            }

            // Escape يرجع للقائمة السابقة
            if (const auto* keyPressed = event->getIf<Event::KeyPressed>())
                if (keyPressed->code == Keyboard::Key::Escape)
                    return;

            // أحداث الضغط بالماوس
            if (const auto* mouseBtn = event->getIf<Event::MouseButtonReleased>())
            {
                if (mouseBtn->button == Mouse::Button::Left)
                {
                    Vector2f mousePos = window.mapPixelToCoords(Mouse::getPosition(window), view); // موضع الماوس في عالم اللعبة

                    if (showQuitPopup)
                    {
                        // زرار 1: Save & Quit → الرجوع للقائمة الرئيسية
                        if (quitPopupBtn1Sprite && quitPopupBtn1Sprite->getGlobalBounds().contains(mousePos))
                        {
                            showQuitPopup = false;
                            MainMenu();
                            return;
                        }
                        // زرار 2: Quit without Save → الرجوع للقائمة الرئيسية
                        if (quitPopupBtn2Sprite && quitPopupBtn2Sprite->getGlobalBounds().contains(mousePos))
                        {
                            showQuitPopup = false;
                            MainMenu();
                            return;
                        }
                        // زرار 3: Don't Quit → إغلاق البوب أب والاستمرار في اللعبة
                        if (quitPopupBtn3Sprite && quitPopupBtn3Sprite->getGlobalBounds().contains(mousePos))
                            showQuitPopup = false;
                    }
                    else
                    {
                        // زرار Options → فتح قائمة الإعدادات
                        if (gsBtn1Sprite && gsBtn1Sprite->getGlobalBounds().contains(mousePos))
                            OptionsMenu();

                        // زرار Quit → إظهار نافذة تأكيد الخروج
                        if (gsBtn2Sprite && gsBtn2Sprite->getGlobalBounds().contains(mousePos))
                            showQuitPopup = true;
                    }
                }
            }
        }

        UpdateGameScreen(); // تحديث حالة الأزرار والـ hover
        DrawGameScreen();   // رسم كل العناصر على الشاشة
    }
}

// ==========================================
// ===== StartGameScreen - تهيئة العناصر =====
// ==========================================
void StartGameScreen(int level)
{
    selectedLevel = level;          // حفظ رقم اللفل المختار
    gameScreenActive = true;        // تفعيل حالة شاشة اللعبة
    window.setFramerateLimit(60);   // تحديد الـ FPS بـ 60
    window.setView(view);           // تطبيق الـ view الحالي

    // ===== تحميل وضبط خلفية الشاشة =====
    if (!gameScreenBgTexture.loadFromFile("Assets/GameScreen/shell_stageinfo.jpg"))
        cout << "Error: shell_stageinfo.jpg\n";
    gameScreenBgTexture.setSmooth(true);
    gameScreenBgSprite = sf::Sprite(gameScreenBgTexture);
    sf::Vector2u bgSize = gameScreenBgTexture.getSize();
    gameScreenBgSprite.setScale({ WindowWidth / (float)bgSize.x, WindowHeight / (float)bgSize.y }); // تمديد الخلفية لتملأ الشاشة
    gameScreenBgSprite.setPosition({ 0.f, 0.f });                                                    // وضع الخلفية في الزاوية العلوية اليسرى

    // ===== تحميل وضبط زرار Options (btn1) =====
    if (!gsBtn1NormalTex.loadFromFile("Assets/GameScreen/option1.png"))
        cout << "Error: option1.png\n";
    if (!gsBtn1HoverTex.loadFromFile("Assets/GameScreen/option2.png"))
        cout << "Error: option2.png\n";
    gsBtn1NormalTex.setSmooth(true);
    gsBtn1HoverTex.setSmooth(true);
    gsBtn1Sprite = sf::Sprite(gsBtn1NormalTex);
    gsBtn1Sprite->setOrigin({ gsBtn1NormalTex.getSize().x / 2.f, gsBtn1NormalTex.getSize().y / 2.f }); // نقطة الأصل في المنتصف
    gsBtn1Sprite->setPosition({ GS_BTN1_X, GS_BTN1_Y });                                               // وضع الزرار في مكانه
    gsBtn1Sprite->setScale({ GS_BTN1_SCALE, GS_BTN1_SCALE });                                          // ضبط الحجم

    // ===== تحميل وضبط زرار Quit (btn2) =====
    if (!gsBtn2NormalTex.loadFromFile("Assets/GameScreen/quit1.png"))
        cout << "Error: quit1.png\n";
    if (!gsBtn2HoverTex.loadFromFile("Assets/GameScreen/quit2.png"))
        cout << "Error: quit2.png\n";
    gsBtn2NormalTex.setSmooth(true);
    gsBtn2HoverTex.setSmooth(true);
    gsBtn2Sprite = sf::Sprite(gsBtn2NormalTex);
    gsBtn2Sprite->setOrigin({ gsBtn2NormalTex.getSize().x / 2.f, gsBtn2NormalTex.getSize().y / 2.f }); // نقطة الأصل في المنتصف
    gsBtn2Sprite->setPosition({ GS_BTN2_X, GS_BTN2_Y });                                               // وضع الزرار في مكانه
    gsBtn2Sprite->setScale({ GS_BTN2_SCALE, GS_BTN2_SCALE });                                          // ضبط الحجم

    // ===== تحميل وضبط زرار Continue (btn3) =====
    if (!gsBtn3NormalTex.loadFromFile("Assets/GameScreen/continue1.png"))
        cout << "Error: continue1.png\n";
    if (!gsBtn3HoverTex.loadFromFile("Assets/GameScreen/continue_large-removebg-preview.png"))
        cout << "Error: continue_large.png\n";
    gsBtn3NormalTex.setSmooth(true);
    gsBtn3HoverTex.setSmooth(true);
    gsBtn3Sprite = sf::Sprite(gsBtn3NormalTex);
    gsBtn3Sprite->setOrigin({ gsBtn3NormalTex.getSize().x / 2.f, gsBtn3NormalTex.getSize().y / 2.f }); // نقطة الأصل في المنتصف
    gsBtn3Sprite->setPosition({ GS_BTN3_X, GS_BTN3_Y });                                               // وضع الزرار في مكانه
    gsBtn3Sprite->setScale({ GS_BTN3_SCALE, GS_BTN3_SCALE });                                          // ضبط الحجم

    // ===== تهيئة نافذة تأكيد الخروج =====
    showQuitPopup = false; // إخفاء نافذة الخروج عند البداية

    // خلفية نافذة الخروج
    if (!quitPopupBgTex.loadFromFile("Assets/GameScreen/shell_shared_popupbg_medium-removebg-preview.png"))
        cout << "Error: quit_popup_bg\n";
    quitPopupBgTex.setSmooth(true);
    quitPopupBgSprite = sf::Sprite(quitPopupBgTex);
    quitPopupBgSprite->setOrigin({ quitPopupBgTex.getSize().x / 2.f, quitPopupBgTex.getSize().y / 2.f }); // نقطة الأصل في المنتصف
    quitPopupBgSprite->setPosition({ QUIT_POPUP_X, QUIT_POPUP_Y });                                        // وضع الخلفية في المنتصف

    // عنوان نافذة الخروج
    if (!quitPopupTitleTex.loadFromFile("Assets/GameScreen/shell_quitgame_hdr-removebg-preview.png"))
        cout << "Error: quit_popup_title\n";
    quitPopupTitleTex.setSmooth(true);
    quitPopupTitleSprite = sf::Sprite(quitPopupTitleTex);
    quitPopupTitleSprite->setOrigin({ quitPopupTitleTex.getSize().x / 2.f, quitPopupTitleTex.getSize().y / 2.f }); // نقطة الأصل في المنتصف
    quitPopupTitleSprite->setPosition({ QUIT_POPUP_X, 180.f });                                                    // وضع العنوان فوق النافذة

    // نص السؤال في نافذة الخروج
    quitPopupLine1 = Text(font, "Are you sure you want to quit?", 22);
    quitPopupLine1.setFillColor(Color::White);
    {
        FloatRect b = quitPopupLine1.getLocalBounds();
        quitPopupLine1.setOrigin({ b.size.x / 2.f, b.size.y / 2.f });   // توسيط النص
        quitPopupLine1.setPosition({ QUIT_POPUP_X, 230.f });
    }

    // نص التوجيه في نافذة الخروج
    quitPopupLine2 = Text(font, "(Select quit option)", 18);
    quitPopupLine2.setFillColor(Color(200, 230, 200));
    {
        FloatRect b = quitPopupLine2.getLocalBounds();
        quitPopupLine2.setOrigin({ b.size.x / 2.f, b.size.y / 2.f });   // توسيط النص
        quitPopupLine2.setPosition({ QUIT_POPUP_X, 250.f });
    }

    // زرار 1: Save & Quit
    if (!quitPopupBtn1NormalTex.loadFromFile("Assets/GameScreen/Screenshot_2026-04-25_222549-removebg-preview.png"))
        cout << "Error: save_quit_normal\n";
    if (!quitPopupBtn1HoverTex.loadFromFile("Assets/GameScreen/Screenshot_2026-04-25_164334-removebg-preview.png"))
        cout << "Error: save_quit_hover\n";
    quitPopupBtn1Sprite = sf::Sprite(quitPopupBtn1NormalTex);
    quitPopupBtn1Sprite->setOrigin({ quitPopupBtn1NormalTex.getSize().x / 2.f, quitPopupBtn1NormalTex.getSize().y / 2.f }); // نقطة الأصل في المنتصف
    quitPopupBtn1Sprite->setPosition({ QUIT_POPUP_X, QUIT_BTN1_Y });                                                        // وضع الزرار
    quitPopupBtn1Sprite->setScale({ QUIT_BTN_SCALE, QUIT_BTN_SCALE });                                                      // ضبط الحجم

    // زرار 2: Quit without Save
    if (!quitPopupBtn2NormalTex.loadFromFile("Assets/GameScreen/Screenshot_2026-04-25_223151-removebg-preview.png"))
        cout << "Error: quit_nosave_normal\n";
    if (!quitPopupBtn2HoverTex.loadFromFile("Assets/GameScreen/Screenshot_2026-04-25_230020-removebg-preview.png"))
        cout << "Error: quit_nosave_hover\n";
    quitPopupBtn2Sprite = sf::Sprite(quitPopupBtn2NormalTex);
    quitPopupBtn2Sprite->setOrigin({ quitPopupBtn2NormalTex.getSize().x / 2.f, quitPopupBtn2NormalTex.getSize().y / 2.f }); // نقطة الأصل في المنتصف
    quitPopupBtn2Sprite->setPosition({ QUIT_POPUP_X, QUIT_BTN2_Y });                                                        // وضع الزرار
    quitPopupBtn2Sprite->setScale({ QUIT_BTN_SCALE, QUIT_BTN_SCALE });                                                      // ضبط الحجم

    // زرار 3: Don't Quit
    if (!quitPopupBtn3NormalTex.loadFromFile("Assets/GameScreen/Screenshot_2026-04-25_223327-removebg-preview.png"))
        cout << "Error: dontquit_normal\n";
    if (!quitPopupBtn3HoverTex.loadFromFile("Assets/GameScreen/Screenshot_2026-04-25_230106-removebg-preview.png"))
        cout << "Error: dontquit_hover\n";
    quitPopupBtn3Sprite = sf::Sprite(quitPopupBtn3NormalTex);
    quitPopupBtn3Sprite->setOrigin({ quitPopupBtn3NormalTex.getSize().x / 2.f, quitPopupBtn3NormalTex.getSize().y / 2.f }); // نقطة الأصل في المنتصف
    quitPopupBtn3Sprite->setPosition({ QUIT_POPUP_X, QUIT_BTN3_Y });                                                        // وضع الزرار
    quitPopupBtn3Sprite->setScale({ QUIT_BTN_SCALE, QUIT_BTN_SCALE });                                                      // ضبط الحجم

    // ===== تحميل محتوى اللفل الأول =====
    if (level == 1)
    {
        // تحميل الخط المستخدم في نصوص اللفل
        if (!lvl1Font.openFromFile("Assets/Fonts/trebuc.ttf"))
            cout << "Error: lvl1 font\n";
        lvl1Font.setSmooth(true);

        // صورة 1: شخصية Boris
        if (!lvl1Img1Tex.loadFromFile("Assets/GameScreen/Screenshot_2026-04-25_021234-artguru-removebg-preview.png"))
            cout << "Error: lvl1 img1\n";
        lvl1Img1Tex.setSmooth(true);
        lvl1Img1Sprite = sf::Sprite(lvl1Img1Tex);
        lvl1Img1Sprite->setOrigin({ lvl1Img1Tex.getSize().x / 2.f, lvl1Img1Tex.getSize().y / 2.f });
        lvl1Img1Sprite->setPosition({ LVL1_IMG1_X, LVL1_IMG1_Y });
        lvl1Img1Sprite->setScale({ LVL1_IMG1_SCALE, LVL1_IMG1_SCALE });

        // صورة 2: سمكة صغيرة
        if (!lvl1Img2Tex.loadFromFile("Assets/GameScreen/Screenshot_2026-04-24_222635-removebg-preview.png"))
            cout << "Error: lvl1 img2\n";
        lvl1Img2Tex.setSmooth(true);
        lvl1Img2Sprite = sf::Sprite(lvl1Img2Tex);
        lvl1Img2Sprite->setOrigin({ lvl1Img2Tex.getSize().x / 2.f, lvl1Img2Tex.getSize().y / 2.f });
        lvl1Img2Sprite->setPosition({ LVL1_IMG2_X, LVL1_IMG2_Y });
        lvl1Img2Sprite->setScale({ LVL1_IMG2_SCALE, LVL1_IMG2_SCALE });

        // صورة 3: سمكة متوسطة
        if (!lvl1Img3Tex.loadFromFile("Assets/GameScreen/Screenshot_2026-04-24_222758-removebg-preview.png"))
            cout << "Error: lvl1 img3\n";
        lvl1Img3Tex.setSmooth(true);
        lvl1Img3Sprite = sf::Sprite(lvl1Img3Tex);
        lvl1Img3Sprite->setOrigin({ lvl1Img3Tex.getSize().x / 2.f, lvl1Img3Tex.getSize().y / 2.f });
        lvl1Img3Sprite->setPosition({ LVL1_IMG3_X, LVL1_IMG3_Y });
        lvl1Img3Sprite->setScale({ LVL1_IMG3_SCALE, LVL1_IMG3_SCALE });

        // صورة 4: HUD اللعبة
        if (!lvl1Img4Tex.loadFromFile("Assets/GameScreen/hudimage-removebg.png"))
            cout << "Error: lvl1 img4\n";
        lvl1Img4Tex.setSmooth(true);
        lvl1Img4Sprite = sf::Sprite(lvl1Img4Tex);
        lvl1Img4Sprite->setOrigin({ lvl1Img4Tex.getSize().x / 2.f, lvl1Img4Tex.getSize().y / 2.f });
        lvl1Img4Sprite->setPosition({ LVL1_IMG4_X, LVL1_IMG4_Y });
        lvl1Img4Sprite->setScale({ LVL1_IMG4_SCALE, LVL1_IMG4_SCALE });

        // عنوان اللفل
        lvl1TitleText.emplace(lvl1Font);
        lvl1TitleText->setString("1. Level 1");
        lvl1TitleText->setCharacterSize(LVL1_TITLE_SIZE);
        lvl1TitleText->setFillColor(Color(255, 220, 50));           // لون ذهبي
        lvl1TitleText->setOutlineColor(Color(80, 40, 0));
        lvl1TitleText->setOutlineThickness(2.f);
        {
            FloatRect b = lvl1TitleText->getLocalBounds();
            lvl1TitleText->setOrigin({ b.size.x / 2.f, b.size.y / 2.f }); // توسيط النص
            lvl1TitleText->setPosition({ LVL1_TITLE_X, LVL1_TITLE_Y });
        }

        // السطر الثاني تحت العنوان
        lvl1SubtitleText.emplace(lvl1Font);
        lvl1SubtitleText->setString("Time to get your fins wet!");
        lvl1SubtitleText->setCharacterSize(LVL1_SUBTITLE_SIZE);
        lvl1SubtitleText->setFillColor(Color(255, 255, 255));
        lvl1SubtitleText->setOutlineColor(Color(30, 30, 30));
        lvl1SubtitleText->setOutlineThickness(1.5f);
        {
            FloatRect b = lvl1SubtitleText->getLocalBounds();
            lvl1SubtitleText->setOrigin({ b.size.x / 2.f, b.size.y / 2.f }); // توسيط النص
            lvl1SubtitleText->setPosition({ LVL1_SUBTITLE_X, LVL1_SUBTITLE_Y });
        }

        // نص وصف القصة
        lvl1DescText.emplace(lvl1Font);
        lvl1DescText->setString(
            "Meet Boris the Butterfly Fish. Boris lives in the\n"
            "warm waters of the Sandy Shoal - a beautiful\n"
            "area off the Frenzy Coast. Enjoy the sights, but\n"
            "don't get too comfortable... A fish still needs his\n"
            "lunch, and it's a fish-eat-fish world out there!"
        );
        lvl1DescText->setCharacterSize(LVL1_DESC_SIZE);
        lvl1DescText->setFillColor(Color(240, 240, 240));
        lvl1DescText->setOutlineColor(Color(20, 20, 20));
        lvl1DescText->setOutlineThickness(1.f);
        lvl1DescText->setPosition({ LVL1_DESC_X, LVL1_DESC_Y }); // وضع النص في مكانه

        // عنوان قسم How to Play
        lvl1HTPText.emplace(lvl1Font);
        lvl1HTPText->setString("How to play:");
        lvl1HTPText->setCharacterSize(LVL1_HTP_SIZE);
        lvl1HTPText->setFillColor(Color(240, 240, 240));
        lvl1HTPText->setOutlineColor(Color(20, 20, 20));
        lvl1HTPText->setOutlineThickness(1.5f);
        lvl1HTPText->setPosition({ LVL1_HTP_X, LVL1_HTP_Y }); // وضع العنوان في مكانه

        // نص تعليمات اللعب
        lvl1TipsText.emplace(lvl1Font);
        lvl1TipsText->setString(
            "- Use your mouse to control Boris.\n"
            "- Eat fish that are smaller than you.\n"
            "- Avoid anything that's larger than you.\n"
            "- Eat enough fish and you'll grow bigger!"
        );
        lvl1TipsText->setCharacterSize(LVL1_TIPS_SIZE);
        lvl1TipsText->setFillColor(Color(240, 240, 240));
        lvl1TipsText->setOutlineColor(Color(20, 20, 20));
        lvl1TipsText->setOutlineThickness(1.f);
        lvl1TipsText->setPosition({ LVL1_TIPS_X, LVL1_TIPS_Y }); // وضع النص في مكانه

        // النص السفلي التشجيعي
        lvl1FooterText.emplace(lvl1Font);
        lvl1FooterText->setString("Warm up those chompers and have some fun!");
        lvl1FooterText->setCharacterSize(LVL1_FOOTER_SIZE);
        lvl1FooterText->setFillColor(Color(240, 240, 240));
        lvl1FooterText->setOutlineColor(Color(20, 20, 20));
        lvl1FooterText->setOutlineThickness(1.f);
        {
            FloatRect b = lvl1FooterText->getLocalBounds();
            lvl1FooterText->setOrigin({ b.size.x / 2.f, b.size.y / 2.f }); // توسيط النص
            lvl1FooterText->setPosition({ LVL1_FOOTER_X, LVL1_FOOTER_Y });
        }
    }

    // ===== تحميل محتوى اللفل الثاني =====
    else if (level == 2)
    {
        // تحميل الخط المستخدم في نصوص اللفل
        if (!lvl2Font.openFromFile("Assets/Fonts/trebuc.ttf"))
            cout << "Error: lvl2 font\n";
        lvl2Font.setSmooth(true);

        // صورة 1: نجمة البحر
        if (!lvl2Img1Tex.loadFromFile("Assets/GameScreen/starbubble1.png"))
            cout << "Error: lvl2 img1\n";
        lvl2Img1Tex.setSmooth(true);
        lvl2Img1Sprite = sf::Sprite(lvl2Img1Tex);
        lvl2Img1Sprite->setOrigin({ lvl2Img1Tex.getSize().x / 2.f, lvl2Img1Tex.getSize().y / 2.f });
        lvl2Img1Sprite->setPosition({ LVL2_IMG1_X, LVL2_IMG1_Y });
        lvl2Img1Sprite->setScale({ LVL2_IMG1_SCALE, LVL2_IMG1_SCALE });

        // صورة 2: أثر اللمعان
        if (!lvl2Img2Tex.loadFromFile("Assets/GameScreen/sparkletrail.png"))
            cout << "Error: lvl2 img2\n";
        lvl2Img2Tex.setSmooth(true);
        lvl2Img2Sprite = sf::Sprite(lvl2Img2Tex);
        lvl2Img2Sprite->setOrigin({ lvl2Img2Tex.getSize().x / 2.f, lvl2Img2Tex.getSize().y / 2.f });
        lvl2Img2Sprite->setPosition({ LVL2_IMG2_X, LVL2_IMG2_Y });
        lvl2Img2Sprite->setScale({ LVL2_IMG2_SCALE, LVL2_IMG2_SCALE });

        // صورة 3: سهم الكليك
        if (!lvl2Img3Tex.loadFromFile("Assets/GameScreen/scr_mouseleftclickarrow-removebg-preview.png"))
            cout << "Error: lvl2 img3\n";
        lvl2Img3Tex.setSmooth(true);
        lvl2Img3Sprite = sf::Sprite(lvl2Img3Tex);
        lvl2Img3Sprite->setOrigin({ lvl2Img3Tex.getSize().x / 2.f, lvl2Img3Tex.getSize().y / 2.f });
        lvl2Img3Sprite->setPosition({ LVL2_IMG3_X, LVL2_IMG3_Y });
        lvl2Img3Sprite->setScale({ LVL2_IMG3_SCALE, LVL2_IMG3_SCALE });

        // صورة 4: سمكة اللفل
        if (!lvl2Img4Tex.loadFromFile("Assets/GameScreen/Screenshot_2026-04-24_223502-removebg-preview.png"))
            cout << "Error: lvl2 img4\n";
        lvl2Img4Tex.setSmooth(true);
        lvl2Img4Sprite = sf::Sprite(lvl2Img4Tex);
        lvl2Img4Sprite->setOrigin({ lvl2Img4Tex.getSize().x / 2.f, lvl2Img4Tex.getSize().y / 2.f });
        lvl2Img4Sprite->setPosition({ LVL2_IMG4_X, LVL2_IMG4_Y });
        lvl2Img4Sprite->setScale({ LVL2_IMG4_SCALE, LVL2_IMG4_SCALE });

        // عنوان اللفل
        lvl2TitleText.emplace(lvl2Font);
        lvl2TitleText->setString("2. Level 2");
        lvl2TitleText->setCharacterSize(LVL2_TITLE_SIZE);
        lvl2TitleText->setFillColor(Color(255, 220, 50));           // لون ذهبي
        lvl2TitleText->setOutlineColor(Color(80, 40, 0));
        lvl2TitleText->setOutlineThickness(2.f);
        {
            FloatRect b = lvl2TitleText->getLocalBounds();
            lvl2TitleText->setOrigin({ b.size.x / 2.f, b.size.y / 2.f }); // توسيط النص
            lvl2TitleText->setPosition({ LVL2_TITLE_X, LVL2_TITLE_Y });
        }

        // السطر الثاني تحت العنوان
        lvl2SubtitleText.emplace(lvl2Font);
        lvl2SubtitleText->setString("A Tasty Treat");
        lvl2SubtitleText->setCharacterSize(LVL2_SUBTITLE_SIZE);
        lvl2SubtitleText->setFillColor(Color(255, 255, 255));
        lvl2SubtitleText->setOutlineColor(Color(30, 30, 30));
        lvl2SubtitleText->setOutlineThickness(1.5f);
        {
            FloatRect b = lvl2SubtitleText->getLocalBounds();
            lvl2SubtitleText->setOrigin({ b.size.x / 2.f, b.size.y / 2.f }); // توسيط النص
            lvl2SubtitleText->setPosition({ LVL2_SUBTITLE_X, LVL2_SUBTITLE_Y });
        }

        // عنوان قسم نجوم البحر
        lvl2Head1Text.emplace(lvl2Font);
        lvl2Head1Text->setString("Watch for Starfish!");
        lvl2Head1Text->setCharacterSize(LVL2_HEAD1_SIZE);
        lvl2Head1Text->setFillColor(Color(240, 240, 240));
        lvl2Head1Text->setOutlineColor(Color(20, 20, 20));
        lvl2Head1Text->setOutlineThickness(1.5f);
        lvl2Head1Text->setPosition({ LVL2_HEAD1_X, LVL2_HEAD1_Y }); // وضع العنوان في مكانه

        // نص وصف نجوم البحر
        lvl2DescText.emplace(lvl2Font);
        lvl2DescText->setString(
            "Tasty STARFISH have been seen floating around\n"
            "these parts. Grab a starfish bubble for some\n"
            "extra points. They won't help Boris get any\n"
            "bigger, but they're sure to grow your score!"
        );
        lvl2DescText->setCharacterSize(LVL2_DESC_SIZE);
        lvl2DescText->setFillColor(Color(240, 240, 240));
        lvl2DescText->setOutlineColor(Color(20, 20, 20));
        lvl2DescText->setOutlineThickness(1.f);
        lvl2DescText->setPosition({ LVL2_DESC_X, LVL2_DESC_Y }); // وضع النص في مكانه

        // عنوان قسم الـ Dash
        lvl2Head2Text.emplace(lvl2Font);
        lvl2Head2Text->setString("How to Dash:");
        lvl2Head2Text->setCharacterSize(LVL2_HEAD2_SIZE);
        lvl2Head2Text->setFillColor(Color(240, 240, 240));
        lvl2Head2Text->setOutlineColor(Color(20, 20, 20));
        lvl2Head2Text->setOutlineThickness(1.5f);
        lvl2Head2Text->setPosition({ LVL2_HEAD2_X, LVL2_HEAD2_Y }); // وضع العنوان في مكانه

        // نص شرح الـ Dash
        lvl2DashText.emplace(lvl2Font);
        lvl2DashText->setString(
            "Tap the left mouse button for a short burst of speed.\n"
            "It doesn't last long, but it might help you out\n"
            "of a tough spot."
        );
        lvl2DashText->setCharacterSize(LVL2_DASH_SIZE);
        lvl2DashText->setFillColor(Color(240, 240, 240));
        lvl2DashText->setOutlineColor(Color(20, 20, 20));
        lvl2DashText->setOutlineThickness(1.f);
        lvl2DashText->setPosition({ LVL2_DASH_X, LVL2_DASH_Y }); // وضع النص في مكانه

        // النص السفلي التحذيري
        lvl2FooterText.emplace(lvl2Font);
        lvl2FooterText->setString("Stay alert, and watch out for sharks!");
        lvl2FooterText->setCharacterSize(LVL2_FOOTER_SIZE);
        lvl2FooterText->setFillColor(Color(240, 240, 240));
        lvl2FooterText->setOutlineColor(Color(20, 20, 20));
        lvl2FooterText->setOutlineThickness(1.f);
        {
            FloatRect b = lvl2FooterText->getLocalBounds();
            lvl2FooterText->setOrigin({ b.size.x / 2.f, b.size.y / 2.f }); // توسيط النص
            lvl2FooterText->setPosition({ LVL2_FOOTER_X, LVL2_FOOTER_Y });
        }
    }

    // ===== تحميل محتوى اللفل الثالث =====
    else if (level == 3)
    {
        // تحميل الخط المستخدم في نصوص اللفل
        if (!lvl3Font.openFromFile("Assets/Fonts/trebuc.ttf"))
            cout << "Error: lvl3 font\n";
        lvl3Font.setSmooth(true);

        // صورة 1: فقاعة Frenzy
        if (!lvl3Img1Tex.loadFromFile("Assets/GameScreen/ffbubble1.png"))
            cout << "Error: lvl3 img1\n";
        lvl3Img1Tex.setSmooth(true);
        lvl3Img1Sprite = sf::Sprite(lvl3Img1Tex);
        lvl3Img1Sprite->setOrigin({ lvl3Img1Tex.getSize().x / 2.f, lvl3Img1Tex.getSize().y / 2.f });
        lvl3Img1Sprite->setPosition({ LVL3_IMG1_X, LVL3_IMG1_Y });
        lvl3Img1Sprite->setScale({ LVL3_IMG1_SCALE, LVL3_IMG1_SCALE });

        // صورة 2: أثر اللمعان
        if (!lvl3Img2Tex.loadFromFile("Assets/GameScreen/sparkletrail.png"))
            cout << "Error: lvl3 img2\n";
        lvl3Img2Tex.setSmooth(true);
        lvl3Img2Sprite = sf::Sprite(lvl3Img2Tex);
        lvl3Img2Sprite->setOrigin({ lvl3Img2Tex.getSize().x / 2.f, lvl3Img2Tex.getSize().y / 2.f });
        lvl3Img2Sprite->setPosition({ LVL3_IMG2_X, LVL3_IMG2_Y });
        lvl3Img2Sprite->setScale({ LVL3_IMG2_SCALE, LVL3_IMG2_SCALE });

        // صورة 3: مقياس الـ Frenzy
        if (!lvl3Img3Tex.loadFromFile("Assets/GameScreen/scr_frenzymeter-removebg-preview.png"))
            cout << "Error: lvl3 img3\n";
        lvl3Img3Tex.setSmooth(true);
        lvl3Img3Sprite = sf::Sprite(lvl3Img3Tex);
        lvl3Img3Sprite->setOrigin({ lvl3Img3Tex.getSize().x / 2.f, lvl3Img3Tex.getSize().y / 2.f });
        lvl3Img3Sprite->setPosition({ LVL3_IMG3_X, LVL3_IMG3_Y });
        lvl3Img3Sprite->setScale({ LVL3_IMG3_SCALE, LVL3_IMG3_SCALE });

        // عنوان اللفل
        lvl3TitleText.emplace(lvl3Font);
        lvl3TitleText->setString("3. Level 3");
        lvl3TitleText->setCharacterSize(LVL3_TITLE_SIZE);
        lvl3TitleText->setFillColor(Color(255, 220, 50));           // لون ذهبي
        lvl3TitleText->setOutlineColor(Color(80, 40, 0));
        lvl3TitleText->setOutlineThickness(2.f);
        {
            FloatRect b = lvl3TitleText->getLocalBounds();
            lvl3TitleText->setOrigin({ b.size.x / 2.f, b.size.y / 2.f }); // توسيط النص
            lvl3TitleText->setPosition({ LVL3_TITLE_X, LVL3_TITLE_Y });
        }

        // السطر الثاني تحت العنوان
        lvl3SubtitleText.emplace(lvl3Font);
        lvl3SubtitleText->setString("Now with even more Frenzy!");
        lvl3SubtitleText->setCharacterSize(LVL3_SUBTITLE_SIZE);
        lvl3SubtitleText->setFillColor(Color(255, 255, 255));
        lvl3SubtitleText->setOutlineColor(Color(30, 30, 30));
        lvl3SubtitleText->setOutlineThickness(1.5f);
        {
            FloatRect b = lvl3SubtitleText->getLocalBounds();
            lvl3SubtitleText->setOrigin({ b.size.x / 2.f, b.size.y / 2.f }); // توسيط النص
            lvl3SubtitleText->setPosition({ LVL3_SUBTITLE_X, LVL3_SUBTITLE_Y });
        }

        // عنوان قسم Frenzy Time
        lvl3Head1Text.emplace(lvl3Font);
        lvl3Head1Text->setString("Frenzy Time!");
        lvl3Head1Text->setCharacterSize(LVL3_HEAD1_SIZE);
        lvl3Head1Text->setFillColor(Color(240, 240, 240));
        lvl3Head1Text->setOutlineColor(Color(20, 20, 20));
        lvl3Head1Text->setOutlineThickness(1.5f);
        lvl3Head1Text->setPosition({ LVL3_HEAD1_X, LVL3_HEAD1_Y }); // وضع العنوان في مكانه

        // نص وصف نظام الـ Frenzy
        lvl3DescText.emplace(lvl3Font);
        lvl3DescText->setString(
            "Now that you're warmed up, it's time to get your Frenzy on.\n"
            "Eat fish quickly to fill up the FRENZY METER. Each time you\n"
            "fill the meter, your Frenzy Multiplier increases and everything\n"
            "you eat is worth more points! Try to reach Mega Frenzy for a 6x bonus!"
        );
        lvl3DescText->setCharacterSize(LVL3_DESC_SIZE);
        lvl3DescText->setFillColor(Color(240, 240, 240));
        lvl3DescText->setOutlineColor(Color(20, 20, 20));
        lvl3DescText->setOutlineThickness(1.f);
        lvl3DescText->setPosition({ LVL3_DESC_X, LVL3_DESC_Y }); // وضع النص في مكانه

        // عنوان قسم Frenzy Boost
        lvl3Head2Text.emplace(lvl3Font);
        lvl3Head2Text->setString("Frenzy Boost");
        lvl3Head2Text->setCharacterSize(LVL3_HEAD2_SIZE);
        lvl3Head2Text->setFillColor(Color(240, 240, 240));
        lvl3Head2Text->setOutlineColor(Color(20, 20, 20));
        lvl3Head2Text->setOutlineThickness(1.5f);
        lvl3Head2Text->setPosition({ LVL3_HEAD2_X, LVL3_HEAD2_Y }); // وضع العنوان في مكانه

        // نص شرح الـ Frenzy Boost
        lvl3BoostText.emplace(lvl3Font);
        lvl3BoostText->setString(
            "Look out for special Frenzy Boost power-ups.\n"
            "Eat a Frenzy Booster to fill up the Frenzy\n"
            "Meter in a single chomp!"
        );
        lvl3BoostText->setCharacterSize(LVL3_BOOST_SIZE);
        lvl3BoostText->setFillColor(Color(240, 240, 240));
        lvl3BoostText->setOutlineColor(Color(20, 20, 20));
        lvl3BoostText->setOutlineThickness(1.f);
        lvl3BoostText->setPosition({ LVL3_BOOST_X, LVL3_BOOST_Y }); // وضع النص في مكانه

        // النص السفلي التحفيزي
        lvl3FooterText.emplace(lvl3Font);
        lvl3FooterText->setString("You have to keep eating to keep the Frenzy Meter full!");
        lvl3FooterText->setCharacterSize(LVL3_FOOTER_SIZE);
        lvl3FooterText->setFillColor(Color(240, 240, 240));
        lvl3FooterText->setOutlineColor(Color(20, 20, 20));
        lvl3FooterText->setOutlineThickness(1.f);
        {
            FloatRect b = lvl3FooterText->getLocalBounds();
            lvl3FooterText->setOrigin({ b.size.x / 2.f, b.size.y / 2.f }); // توسيط النص
            lvl3FooterText->setPosition({ LVL3_FOOTER_X, LVL3_FOOTER_Y });
        }
    }
}

// ==========================================
// ===== UpdateGameScreen - تحديث الـ hover =====
// ==========================================
void UpdateGameScreen()
{
    Vector2f mousePos = window.mapPixelToCoords(Mouse::getPosition(window), view); // موضع الماوس الحالي

    // hover زرار Options
    if (gsBtn1Sprite && gsBtn1Sprite->getGlobalBounds().contains(mousePos))
    {
        gsBtn1Sprite->setTexture(gsBtn1HoverTex);                                                   // تغيير للصورة المضيئة
        gsBtn1Sprite->setOrigin({ gsBtn1HoverTex.getSize().x / 2.f, gsBtn1HoverTex.getSize().y / 2.f });
    }
    else if (gsBtn1Sprite)
    {
        gsBtn1Sprite->setTexture(gsBtn1NormalTex);                                                  // الرجوع للصورة العادية
        gsBtn1Sprite->setOrigin({ gsBtn1NormalTex.getSize().x / 2.f, gsBtn1NormalTex.getSize().y / 2.f });
    }

    // hover زرار Quit
    if (gsBtn2Sprite && gsBtn2Sprite->getGlobalBounds().contains(mousePos))
    {
        gsBtn2Sprite->setTexture(gsBtn2HoverTex);                                                   // تغيير للصورة المضيئة
        gsBtn2Sprite->setOrigin({ gsBtn2HoverTex.getSize().x / 2.f, gsBtn2HoverTex.getSize().y / 2.f });
    }
    else if (gsBtn2Sprite)
    {
        gsBtn2Sprite->setTexture(gsBtn2NormalTex);                                                  // الرجوع للصورة العادية
        gsBtn2Sprite->setOrigin({ gsBtn2NormalTex.getSize().x / 2.f, gsBtn2NormalTex.getSize().y / 2.f });
    }

    // hover زرار Continue (بيستخدم منطقة مستطيلة بدل getGlobalBounds)
    bool btn3Hovered = gsBtn3Sprite &&
        mousePos.x >= GS_BTN3_X - GS_BTN3_HOVER_HALF_W &&  // يسار منطقة الكليك
        mousePos.x <= GS_BTN3_X + GS_BTN3_HOVER_HALF_W &&  // يمين منطقة الكليك
        mousePos.y >= GS_BTN3_Y - GS_BTN3_HOVER_HALF_H &&  // أعلى منطقة الكليك
        mousePos.y <= GS_BTN3_Y + GS_BTN3_HOVER_HALF_H;   // أسفل منطقة الكليك

    if (btn3Hovered)
    {
        gsBtn3Sprite->setTexture(gsBtn3HoverTex);                                                   // تغيير للصورة المضيئة
        gsBtn3Sprite->setOrigin({ gsBtn3HoverTex.getSize().x / 2.f, gsBtn3HoverTex.getSize().y / 2.f });
        gsBtn3Sprite->setPosition({ GS_BTN3_X, GS_BTN3_Y });
        gsBtn3Sprite->setScale({ GS_BTN3_HOVER_SCALE, GS_BTN3_HOVER_SCALE });                      // تكبير الزرار عند hover
    }
    else if (gsBtn3Sprite)
    {
        gsBtn3Sprite->setTexture(gsBtn3NormalTex);                                                  // الرجوع للصورة العادية
        gsBtn3Sprite->setOrigin({ gsBtn3NormalTex.getSize().x / 2.f, gsBtn3NormalTex.getSize().y / 2.f });
        gsBtn3Sprite->setPosition({ GS_BTN3_X, GS_BTN3_Y });
        gsBtn3Sprite->setScale({ GS_BTN3_SCALE, GS_BTN3_SCALE });                                  // إرجاع الحجم الأصلي
    }

    // hover أزرار نافذة الخروج (فقط لو النافذة ظاهرة)
    if (showQuitPopup)
    {
        // hover زرار 1 (Save & Quit)
        if (quitPopupBtn1Sprite && quitPopupBtn1Sprite->getGlobalBounds().contains(mousePos))
        {
            quitPopupBtn1Sprite->setTexture(quitPopupBtn1HoverTex);
            quitPopupBtn1Sprite->setOrigin({ quitPopupBtn1HoverTex.getSize().x / 2.f, quitPopupBtn1HoverTex.getSize().y / 2.f });
        }
        else if (quitPopupBtn1Sprite)
        {
            quitPopupBtn1Sprite->setTexture(quitPopupBtn1NormalTex);
            quitPopupBtn1Sprite->setOrigin({ quitPopupBtn1NormalTex.getSize().x / 2.f, quitPopupBtn1NormalTex.getSize().y / 2.f });
        }

        // hover زرار 2 (Quit without Save)
        if (quitPopupBtn2Sprite && quitPopupBtn2Sprite->getGlobalBounds().contains(mousePos))
        {
            quitPopupBtn2Sprite->setTexture(quitPopupBtn2HoverTex);
            quitPopupBtn2Sprite->setOrigin({ quitPopupBtn2HoverTex.getSize().x / 2.f, quitPopupBtn2HoverTex.getSize().y / 2.f });
        }
        else if (quitPopupBtn2Sprite)
        {
            quitPopupBtn2Sprite->setTexture(quitPopupBtn2NormalTex);
            quitPopupBtn2Sprite->setOrigin({ quitPopupBtn2NormalTex.getSize().x / 2.f, quitPopupBtn2NormalTex.getSize().y / 2.f });
        }

        // hover زرار 3 (Don't Quit)
        if (quitPopupBtn3Sprite && quitPopupBtn3Sprite->getGlobalBounds().contains(mousePos))
        {
            quitPopupBtn3Sprite->setTexture(quitPopupBtn3HoverTex);
            quitPopupBtn3Sprite->setOrigin({ quitPopupBtn3HoverTex.getSize().x / 2.f, quitPopupBtn3HoverTex.getSize().y / 2.f });
        }
        else if (quitPopupBtn3Sprite)
        {
            quitPopupBtn3Sprite->setTexture(quitPopupBtn3NormalTex);
            quitPopupBtn3Sprite->setOrigin({ quitPopupBtn3NormalTex.getSize().x / 2.f, quitPopupBtn3NormalTex.getSize().y / 2.f });
        }
    }
}

// ==========================================
// ===== DrawGameScreen - رسم العناصر =====
// ==========================================
void DrawGameScreen()
{
    window.setView(view);
    window.clear(Color::Black); // مسح الشاشة

    window.draw(gameScreenBgSprite); // رسم الخلفية
    if (gsBtn1Sprite) window.draw(*gsBtn1Sprite); // رسم زرار Options
    if (gsBtn2Sprite) window.draw(*gsBtn2Sprite); // رسم زرار Quit
    if (gsBtn3Sprite) window.draw(*gsBtn3Sprite); // رسم زرار Continue

    if (showQuitPopup)
    {
        // رسم نافذة تأكيد الخروج
        if (quitPopupBgSprite)    window.draw(*quitPopupBgSprite);    // خلفية النافذة
        if (quitPopupTitleSprite) window.draw(*quitPopupTitleSprite); // عنوان النافذة
        if (quitPopupBtn1Sprite)  window.draw(*quitPopupBtn1Sprite);  // زرار Save & Quit
        if (quitPopupBtn2Sprite)  window.draw(*quitPopupBtn2Sprite);  // زرار Quit without Save
        if (quitPopupBtn3Sprite)  window.draw(*quitPopupBtn3Sprite);  // زرار Don't Quit
        window.draw(quitPopupLine1); // نص "Are you sure?"
        window.draw(quitPopupLine2); // نص "(Select quit option)"
    }
    else if (selectedLevel == 1)
    {
        // رسم صور Level 1
        if (lvl1Img1Sprite) window.draw(*lvl1Img1Sprite); // شخصية Boris
        if (lvl1Img2Sprite) window.draw(*lvl1Img2Sprite); // سمكة صغيرة
        if (lvl1Img3Sprite) window.draw(*lvl1Img3Sprite); // سمكة متوسطة
        if (lvl1Img4Sprite) window.draw(*lvl1Img4Sprite); // HUD اللعبة
        // رسم نصوص Level 1
        if (lvl1TitleText)    window.draw(*lvl1TitleText);    // عنوان اللفل
        if (lvl1SubtitleText) window.draw(*lvl1SubtitleText); // السطر تحت العنوان
        if (lvl1DescText)     window.draw(*lvl1DescText);     // نص الوصف
        if (lvl1HTPText)      window.draw(*lvl1HTPText);      // عنوان How to Play
        if (lvl1TipsText)     window.draw(*lvl1TipsText);     // نص التعليمات
        if (lvl1FooterText)   window.draw(*lvl1FooterText);   // النص السفلي
    }
    else if (selectedLevel == 2)
    {
        // رسم صور Level 2
        if (lvl2Img1Sprite) window.draw(*lvl2Img1Sprite); // نجمة البحر
        if (lvl2Img2Sprite) window.draw(*lvl2Img2Sprite); // أثر اللمعان
        if (lvl2Img3Sprite) window.draw(*lvl2Img3Sprite); // سهم الكليك
        if (lvl2Img4Sprite) window.draw(*lvl2Img4Sprite); // سمكة اللفل
        // رسم نصوص Level 2
        if (lvl2TitleText)    window.draw(*lvl2TitleText);    // عنوان اللفل
        if (lvl2SubtitleText) window.draw(*lvl2SubtitleText); // السطر تحت العنوان
        if (lvl2Head1Text)    window.draw(*lvl2Head1Text);    // عنوان Watch for Starfish
        if (lvl2DescText)     window.draw(*lvl2DescText);     // نص وصف نجوم البحر
        if (lvl2Head2Text)    window.draw(*lvl2Head2Text);    // عنوان How to Dash
        if (lvl2DashText)     window.draw(*lvl2DashText);     // نص شرح الـ Dash
        if (lvl2FooterText)   window.draw(*lvl2FooterText);   // النص السفلي
    }
    else if (selectedLevel == 3)
    {
        // رسم صور Level 3
        if (lvl3Img1Sprite) window.draw(*lvl3Img1Sprite); // فقاعة Frenzy
        if (lvl3Img2Sprite) window.draw(*lvl3Img2Sprite); // أثر اللمعان
        if (lvl3Img3Sprite) window.draw(*lvl3Img3Sprite); // مقياس الـ Frenzy
        // رسم نصوص Level 3
        if (lvl3TitleText)    window.draw(*lvl3TitleText);    // عنوان اللفل
        if (lvl3SubtitleText) window.draw(*lvl3SubtitleText); // السطر تحت العنوان
        if (lvl3Head1Text)    window.draw(*lvl3Head1Text);    // عنوان Frenzy Time
        if (lvl3DescText)     window.draw(*lvl3DescText);     // نص وصف الـ Frenzy
        if (lvl3Head2Text)    window.draw(*lvl3Head2Text);    // عنوان Frenzy Boost
        if (lvl3BoostText)    window.draw(*lvl3BoostText);    // نص شرح الـ Boost
        if (lvl3FooterText)   window.draw(*lvl3FooterText);   // النص السفلي
    }

    window.display(); // عرض كل اللي اتم رسمه
}