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
void StartSwitchUser();
void CreateButton(Sprite &sprite, Texture &texture, const string &filePath, // A Template for creating sprites
                  float xPosition,
                  float yPositon, float scaleX, float scaleY);
void SetupButtonText(Text &text, const string &str, Sprite &button); // A Template for Creating Text for buttons
void UpdateSwitchUser();
void HoverButton(Sprite &button, const Texture &normalTex,
                 const Texture &highlightTex, Vector2f mousePos); // A Template for Hovering Over Buttons
void EnterYourName();
void RefreshUsersList();
void DeleteUser();
void FullList();       // If the List is Full -> (max is 7 users)
void DupplicateUser(); // If the user tried to enter a name that already exists
void DisplaySwitchUser();

float getRandom(float min, float max)
{
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dist(min, max);
    return dist(gen);
}

// --- المتغيرات العالمية ---
float WindowWidth = 1920;                                                                                    // عرض الشاشة الثابت
float WindowHeight = 1080;                                                                                   // طول الشاشة الثابت
RenderWindow window(VideoMode({(unsigned int)WindowWidth, (unsigned int)WindowHeight}), "Feeding Frenzy 2"); // فتح الشاشة كاملة
View view({400.f, 300.f}, Vector2f(window.getSize()));                                                       // الكاميرا اللي بتظبط أبعاد اللعبة

// =================================== Loading Screen =================================

Shader swayShader;                // "فلتر" حركة المية الهادية
float deltaTime;                  // الوقت بين كل فريم والتاني
float startX = WindowWidth / 2.f; // نقطة بداية رص الحروف في النص
float spacing = 65.0f;            // المسافة الفاضية بين كل حرف وتاني
float width_bubble_global;        // عرض الفقاعة الكبيرة اللي ورا الحرف
float endTimer = 0.0f;            // تايمر بيبدأ بعد ما الحروف تفرقع
bool transitionStarted = false;   // علامة إننا بدأنا وقت الانتظار الأخير

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

AmbientBubble ventBubbles[60]; // مصفوفة فيها 60 فقاعة للجو
float bubbleTimer = 0.0f;      // تايمر عشان نطلع فقاقيع بالعرض

Particle sparks[35];            // 35 شرارة للانفجار
Particle letterBubbles[35];     // 35 فقاعة صغيرة للانفجار
bool exploded = false;          // هل الفرقعة حصلت خلاص؟
bool particlesFinished = false; // هل كل آثار الفرقعة اختفت؟
float timerLeft = 0.0f;         // تايمر للمرجان اللي على الشمال
float timerRight = 0.0f;        // تايمر للمرجان اللي على اليمين

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
// Barracuda FISH  Textures & Sprites
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
        sprite.move({velocityX_AXIS, 0});

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

//================================= Switch User ==================================

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

int main()
{
    cout << "SFML 3.0 and Standard Library are working!" << endl;
    StartLoadingScreen();
    Clock clock;
    Clock totalClock;

    view.setSize({800.f, 600.f});
    view.setCenter({400.f, 300.f});

    // ── Loading Screen Loop ──
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
                float viewRatio = 800.f / 600.f;
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

        if (DrawLoadingScreen(totalTime)) // returns true when loading is done
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
{ // دالة تجهيز كل حاجة في البداية

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
    fontTexture.setSmooth(true); // تنعيم الحروف

    float cellWidth = 19.4;          // عرض الحرف الواحد في الصورة
    float cellHeight = 35.5;         // طول الحرف الواحد في الصورة
    Color targetColor(160, 211, 74); // اللون الأخضر الفوسفوري بتاع اللعبة

    s_L.setTexture(fontTexture);                                                                                   // اربط الحرف L بالصورة
    s_L.setTextureRect(IntRect({(int)(1 * cellWidth), (int)(3 * cellHeight)}, {(int)cellWidth, (int)cellHeight})); // قص مكانه
    s_L.setColor(targetColor);                                                                                     // لونه بالأخضر
    s_L.setOrigin({cellWidth / 2.f, cellHeight / 2.f});                                                            // خلي السنتر في النص

    s_O.setTexture(fontTexture);                                                    // اربط الحرف O
    s_O.setTextureRect(IntRect({(int)(4 * 20), (int)(3 * 35.7)}, {20, (int)35.7})); // قص مكانه
    s_O.setColor(targetColor);                                                      // لونه
    s_O.setOrigin({10.f, 35.7f / 2.f});                                             // السنتر في النص

    s_A.setTexture(fontTexture);                                                                // اربط الحرف A
    s_A.setTextureRect(IntRect({(int)(3 * 20), (int)(2 * cellHeight)}, {20, (int)cellHeight})); // قص مكانه
    s_A.setColor(targetColor);                                                                  // لونه
    s_A.setOrigin({10.f, cellHeight / 2.f});                                                    // السنتر في النص

    s_D.setTexture(fontTexture);                                                                // اربط الحرف D
    s_D.setTextureRect(IntRect({(int)(6 * 20), (int)(2 * cellHeight)}, {20, (int)cellHeight})); // قص مكانه
    s_D.setColor(targetColor);                                                                  // لونه
    s_D.setOrigin({10.f, cellHeight / 2.f});                                                    // السنتر في النص

    s_I.setTexture(fontTexture);                                                                        // اربط الحرف I
    s_I.setTextureRect(IntRect({(int)(10.39 * 20 + 4), (int)(2 * cellHeight)}, {12, (int)cellHeight})); // قص مكانه بدقة
    s_I.setColor(targetColor);                                                                          // لونه
    s_I.setOrigin({6.f, cellHeight / 2.f});                                                             // السنتر في النص

    s_N.setTexture(fontTexture);                                                                // اربط الحرف N
    s_N.setTextureRect(IntRect({(int)(3 * 20), (int)(3 * cellHeight)}, {20, (int)cellHeight})); // قص مكانه
    s_N.setColor(targetColor);                                                                  // لونه
    s_N.setOrigin({10.f, cellHeight / 2.f});                                                    // السنتر في النص

    s_G.setTexture(fontTexture);                                                      // اربط الحرف G
    s_G.setTextureRect(IntRect({174, (int)(2 * cellHeight)}, {18, (int)cellHeight})); // قص مكانه
    s_G.setColor(targetColor);                                                        // لونه
    s_G.setOrigin({9.f, cellHeight / 2.f});                                           // السنتر في النص

    bubbleSprite.setTexture(bubbleTexture);                                                                           // اربط فقاعة الحروف
    bubbleSprite.setScale({0.7f, 0.7f});                                                                              // صغر حجمها شوية
    bubbleSprite.setOrigin({bubbleSprite.getLocalBounds().size.x / 2.f, bubbleSprite.getLocalBounds().size.y / 2.f}); // سنترها
    width_bubble_global = bubbleSprite.getGlobalBounds().size.x;                                                      // احفظ عرضها للعمليات
    bubbleTexture.setSmooth(true);                                                                                    // تنعيم

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

    loadBar_sprite.setTexture(loadBar);                                                                                     // اربط الشريط المليان
    loadBar_sprite.setOrigin({loadBar_sprite.getLocalBounds().size.x / 2.f, loadBar_sprite.getLocalBounds().size.y / 2.f}); // سنتره
    loadBar_sprite.setPosition({WindowWidth / 2.f, WindowHeight - 85.f});                                                   // حطه في نفس مكان البرواز
    loadBar_sprite.setScale({width / loadBar.getSize().x, height / loadBar.getSize().y});                                   // ظبط حجمه
    loadBar.setSmooth(true);                                                                                                // تنعيم

    bubbleSmallTex.setSmooth(true); // تنعيم الفقاقيع الصغيرة
    spark.setSmooth(true);          // تنعيم الشرارات
}

void StartLoadingScreen()
{                     // دالة البدء
    loading_screen(); // شغل دالة التحميل
}

void UpdateLoadingScreen()
{ // دالة التحديث (فاضية حالياً)
}

bool DrawLoadingScreen(float totalTime)
{                                                        // دالة الرسم (بتتكرر كل فريم)
    window.clear(Color::Black);                          // امسح الشاشة وخليها سودة
    swayShader.setUniform("time", totalTime);            // ابعت الوقت للشيدر عشان المية تتحرك
    Background_Loading.setSmooth(true);                  // تأكيد التنعيم
    window.draw(Background_Loading_sprite, &swayShader); // ارسم الخلفية وعليها حركة المية
    window.draw(Game_Icon_sprite);                       // ارسم لوجو اللعبة
    window.draw(POPCAP_Logo_sprite);                     // ارسم لوجو الشركة
    window.draw(LogoWood_sprite);                        // ارسم الخشبة

    float loadingStartTime = 1.0f;                          // وقت بداية التحميل (بعد ثانية)
    float progress = (totalTime - loadingStartTime) / 5.0f; // النسبة (على مدار 5 ثواني)
    if (progress < 0)
        progress = 0; // لو لسه مجاش الوقت خليها صفر
    if (progress > 1.0f)
        progress = 1.0f; // لو خلصت ثبتها على 100%

    Vector2u textureSize = loadBar.getSize();                                           // مقاس صورة الشريط
    int currentWidth = (int)(textureSize.x * progress);                                 // العرض اللي المفروض يظهر
    loadBar_sprite.setTextureRect(IntRect({0, 0}, {currentWidth, (int)textureSize.y})); // قص الجزء المليان بس

    window.draw(loadBar_empty_sprite); // ارسم البرواز الفاضي
    window.draw(loadBar_sprite);       // ارسم الجزء المليان فوقه

    startX = WindowWidth / 2.0f - (3.5f * spacing) + 20.f;          // سنتر الكلمة بالعرض
    Sprite *letters[] = {&s_L, &s_O, &s_A, &s_D, &s_I, &s_N, &s_G}; // لستة بالحروف

    for (int i = 0; i < 7; i++)
    {                                                            // لف على الـ 7 حروف
        float startDelay = 1.5f;                                 // تأخير بداية ظهور الحروف
        float letterDelay = i * 0.4f;                            // تأخير بين كل حرف والتاني
        float letterTime = totalTime - startDelay - letterDelay; // الوقت الخاص بالحرف ده

        float popScale = 0.0f; // الحجم بيبدأ من صفر
        if (letterTime > 0)
        {                                                                                    // لو جه ميعاد ظهوره
            popScale = (letterTime < 0.4f) ? sin(letterTime * (3.14f / 0.4f)) * 1.2f : 1.0f; // حركة الـ Pop (يكبر ويرجع)
            if (popScale < 1.0f && letterTime > 0.2f)
                popScale = 1.0f; // تثبيته بعد الحركة
        }

        float curX = startX + (i * spacing);                                                  // مكانه الأفقي
        float curY = (WindowHeight / 2.0f - 40.f) + sin(totalTime * 4.0f + i * 0.8f) * 12.0f; // حركة الموج لفوق وتحت

        bubbleSprite.setScale({0.7f * popScale, 0.7f * popScale});  // حجم الفقاعة مع الـ Pop
        letters[i]->setScale({0.92f * popScale, 0.92f * popScale}); // حجم الحرف مع الـ Pop
        bubbleSprite.setPosition({curX, curY});                     // مكان الفقاعة
        letters[i]->setPosition({curX, curY});                      // مكان الحرف

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

            letterBubbles[i].active = true; // فعل فقاعة الفرقعة
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

    timerLeft += deltaTime;   // زود تايمر الشمال
    timerRight += deltaTime;  // زود تايمر اليمين
    bubbleTimer += deltaTime; // زود تايمر الجو العام

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
                ventBubbles[i].vx = -((float)(rand() % 30 + 15) / 10.f); // رايحة شمال
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
            return true; // ← loading screen DONE
        }
    }

    window.display();
    return false; // ← still loading
}

void MainMenu()
{
    StartMainMenu();
    while (window.isOpen())
    {
        // totaltime += dt;
        while (const optional event = window.pollEvent())
        {
            if (event->is<Event::Closed>() || Keyboard::isKeyPressed(Keyboard::Key::Escape))
            {
                window.close();
            }
            if (auto mouseEvent = event->getIf<Event::MouseButtonReleased>())
                if (mouseEvent->button == Mouse::Button::Left)
                {
                    Vector2f mousePos = window.mapPixelToCoords(Mouse::getPosition(window));
                    if (switchuserbutton.getGlobalBounds().contains(mousePos))
                        SwitchUser();
                }
        }
        background.setTexture(mainbackground);

        UpdateMainMenu();
        DrawMainMenu();
    }
}
void StartMainMenu()
{
    srand(time(0));
    // creating small fish objects
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
        {
            obj.sprite.setPosition({1100, 360});
        }
        if (i == 2)
        {
            obj.sprite.setPosition({900, 360});
        }
        obj.sprite.setScale({float(obj.changedir * 0.2), 0.2});
        obj.velocity = Vector2f({obj.velocityX_AXIS, 0});
        smallfishs.push_back(obj);
    }

    // frame rate limit
    window.setFramerateLimit(30);
    // set up Textures smooth
    logo.setSmooth(true);
    mainbackground.setSmooth(true);
    // fish22.setSmooth(true);
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

    // Barracudatex.setSmooth(true);

    // setting up sprites
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
    highscorebutton.setPosition(Vector2f({window.getSize().x / 2.0f + -150, window.getSize().y / 2.0f + 140}));
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

    // Barracuda.setScale({2,2});
}

void UpdateMainMenu()
{
    float smallfishsvelocityYaxis = getRandom(-2.f, 2.f);
    // COLLIDERS UPDATE
    Barracudacollieder.setPosition(Vector2f(Barracuda.getPosition().x, Barracuda.getPosition().y + 50));
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
        if (obj.sprite.getPosition().x == window.getPosition().x - 580 || obj.sprite.getPosition().x == window.getPosition().y + 580)
        {
            obj.velocityX_AXIS *= -1;
            obj.changedir *= -1;

            obj.velocity = Vector2f({obj.velocityX_AXIS, 0});
            obj.sprite.setScale({float(0.2 * obj.changedir), 0.2});
        }
    }
    MainMenuFishAnimation();
    ChangingButtonShape();
}

void DrawMainMenuBackground()
{
    window.clear();
    window.draw(background);
    window.draw(Greenfish);
    // window.draw(MFcollieder);
    // window.draw(QueenTriggercollieder);

    window.draw(QueenTrigger);

    window.draw(Minowfish);
    for (auto &obj : smallfishs)
    {
        window.draw(obj.sprite);
    }
    window.draw(n);

    // window.draw(Barracudacollieder);

    window.draw(Barracuda);
    window.draw(b);
}

void DrawMainMenu()
{
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
void ChangingButtonShape()
{
    Vector2i mouseLocalPos = Mouse::getPosition(window); // Get the mouse position relative to the window
    Vector2f mouseWorldPos = window.mapPixelToCoords(mouseLocalPos);
    // Check if the mouse is hovering over the start game button
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
    {
        timeattackbutton.setTexture(timeattackpressed);
    }
    else
    {
        timeattackbutton.setTexture(timeattacktex);
    }
    if (highscorebutton.getGlobalBounds().contains(mouseWorldPos))
    {
        highscorebutton.setTexture(highscorepressed);
    }
    else
    {
        highscorebutton.setTexture(highscoretex);
    }
    if (optionsbutton.getGlobalBounds().contains(mouseWorldPos))
    {
        optionsbutton.setTexture(optionspressed);
    }
    else
    {
        optionsbutton.setTexture(optionstex);
    }
    if (quitbutton.getGlobalBounds().contains(mouseWorldPos))
    {
        quitbutton.setTexture(quitpressed);
    }
    else
    {
        quitbutton.setTexture(quittex);
    }
    if (switchuserbutton.getGlobalBounds().contains(mouseWorldPos))
    {
        switchuserbutton.setTexture(switchuserpressed);
    }
    else
    {
        switchuserbutton.setTexture(switchusertex);
    }
    if (creditsbutton.getGlobalBounds().contains(mouseWorldPos))
    {
        creditsbutton.setTexture(creditspressed);
    }
    else
    {
        creditsbutton.setTexture(creditstex);
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

void SwitchUser()
{
    StartSwitchUser();
    while (window.isOpen())
    {
        UpdateSwitchUser();
        DisplaySwitchUser();
    }
}

void StartSwitchUser()
{
    for (int i = 0; i < 8; i++)
        UserTexts[i] = nullptr; // Initializing Users with Null

    float X = WindowWidth / 2.f, Y = WindowHeight / 2.f;
    CreateButton(Full, FullTex, "Assets/Switch User/BG.png", X, Y, 0.25, 0.25); // Background of the SwitchUser pop-up window

    X = WindowWidth * 0.37f, Y = WindowHeight * 0.68f;
    CreateButton(NewButton, NewButtonTex, "Assets/Switch User/Button.png", X, Y, 1.5, 1.5);
    if (!NewButtonHLTex.loadFromFile("Assets/Switch User/Button High.png"))
        cout << "Failed to load: ButtonHL" << "\n";

    X = WindowWidth * 0.5f, Y = WindowHeight * 0.68f;
    CreateButton(SelectButton, NewButtonTex, "Assets/Switch User/Button.png", X, Y, 1.5, 1.5);
    if (!NewButtonHLTex.loadFromFile("Assets/Switch User/Button High.png"))
        cout << "Failed to load: ButtonHL" << "\n";

    X = WindowWidth * 0.62f, Y = WindowHeight * 0.68f;
    CreateButton(DeleteButton, NewButtonTex, "Assets/Switch User/Button.png", X, Y, 1.5, 1.5);
    if (!NewButtonHLTex.loadFromFile("Assets/Switch User/Button High.png"))
        cout << "Failed to load: ButtonHL" << "\n";

    if (!btnFont.openFromFile("Assets/Switch User/BarmenoBold.otf"))
        cout << "Can't load font\n";
    SetupButtonText(NewText, "New", NewButton);
    SetupButtonText(SelectText, "Select", SelectButton);
    SetupButtonText(DeleteText, "Delete", DeleteButton);

    X = WindowWidth * 0.5f, Y = WindowHeight * 0.25f;
    CreateButton(Title, TitleTex, "Assets/Switch User/shell_chooseuser_hdr.png", X, Y, 1.5, 1.5);

    // ----------- Checking to see if we already Have Users------------------
    ifstream loadusers("Assets/Switch User/Users_List.txt");
    string name;
    int id;
    while (loadusers >> name >> id and NumberOfUsers < MaxNumberOfUsers)
    {
        users[NumberOfUsers].first = name;
        users[NumberOfUsers].second = id;
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
    text.setCharacterSize(40);
    text.setFillColor(Color(210, 225, 90));
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
    Vector2f mousePos = static_cast<Vector2f>(Mouse::getPosition(window));

    while (auto x = window.pollEvent())
    {
        if (x->is<Event::Closed>())
            window.close();

        if (auto mouseEvent = x->getIf<Event::MouseButtonReleased>())
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
                        NameEntry = 0;
                        isCancelAddingUser = 0;
                        for (auto &user : users) // is the name already there?
                        {
                            if (user.first == InputString)
                            {
                                DupplicateName = 1;
                                break;
                            }
                        }
                        if (DupplicateName) // YES?
                        {
                            DupplicateUser(); // it's dupplicated you can't add it
                            NameEntry = 0;
                            if (!CamefromDupplicate)
                                InputString = "";
                        }
                        if (!DupplicateName) // NO? -> add it
                        {
                            ofstream adduser("Users_List.txt");
                            users[NumberOfUsers].first = InputString;
                            users[NumberOfUsers].second = NumberOfUsers + 1;
                            NumberOfUsers++;
                            InputString = "";

                            for (int i = 0; i < NumberOfUsers; i++)
                                adduser << users[i].first << '\t' << users[i].second << '\n';

                            adduser.close();
                            RefreshUsersList();                       // Refresh The List after each addition
                            CurUser = users[NumberOfUsers - 1].first; //[NumberOfUsers - 1] becuase we already did NumberOfUsers++
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
                }

                for (int i = 0; i < NumberOfUsers; i++)
                {
                    if (UserTexts[i] != nullptr and UserTexts[i]->getGlobalBounds().contains(mousePos))
                    {
                        SelectedUser = i;
                        float X = WindowWidth * 0.5f - 5, Y = UserTexts[i]->getPosition().y - 5.f;
                        SelectUserHL.setPosition({X, Y});
                    }
                }

                if (SelectButton.getGlobalBounds().contains(mousePos) and !NameEntry and !isCancelAddingUser and !isListFull and !DupplicateName)
                {
                    CurUser = users[SelectedUser].first;
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

        if (const auto *typed = x->getIf<Event::TextEntered>())
        {
            char32_t a = typed->unicode;
            if (a == 8 /*ASCII for delete*/ and !InputString.empty())
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

    HoverButton(NewButton, NewButtonTex, NewButtonHLTex, mousePos);
    HoverButton(SelectButton, NewButtonTex, NewButtonHLTex, mousePos);
    HoverButton(DeleteButton, NewButtonTex, NewButtonHLTex, mousePos);
    if (NameEntry and !isConfirmUserDelete)
    {
        HoverButton(DoneAddingUser, NewButtonTex, NewButtonHLTex, mousePos);
        HoverButton(CancelAddingUser, CancelAddingUserTex, CancelAddingUserHLTex, mousePos);
    }
    if (isConfirmUserDelete and !NameEntry)
    {
        HoverButton(YesButton, NewButtonTex, NewButtonHLTex, mousePos);
        HoverButton(NoButton, NewButtonTex, NewButtonHLTex, mousePos);
    }
    if (!NameEntry and !isConfirmUserDelete and isListFull and !DupplicateName)
        HoverButton(FullOKButton, NewButtonTex, NewButtonHLTex, mousePos);

    if (!NameEntry and !isConfirmUserDelete and !isListFull and DupplicateName)
        HoverButton(DupplicateOKButton, NewButtonTex, NewButtonHLTex, mousePos);
}

void HoverButton(Sprite &button, const Texture &normalTex,
                 const Texture &highlightTex, Vector2f mousePos)
{
    if (button.getGlobalBounds().contains(mousePos))
        button.setTexture(highlightTex);
    else
        button.setTexture(normalTex);
}

void EnterYourName()
{
    if (NameEntry)
        return;
    isUserSelected = 0;
    Vector2f mousePos = static_cast<Vector2f>(Mouse::getPosition(window));
    NameEntry = 1;
    unsigned int X = WindowWidth * 0.5f, Y = WindowHeight * 0.5f;
    CreateButton(EnterYourNamebg, EnterYourNameBgTex, "Assets/Switch User/EnterYourName.png", X, Y, 0.25, 0.25);

    if (!Inputfont.openFromFile("Assets/Switch User/trebuc.ttf"))
        cout << "Cant Open Font!";

    DisplayText.setFont(Inputfont);
    DisplayText.setCharacterSize(40);
    DisplayText.setFillColor(Color::White);
    X = WindowWidth * 0.38f, Y = WindowHeight * 0.40f;
    DisplayText.setPosition({(float)X, (float)Y});

    X = WindowWidth * 0.38f, Y = WindowHeight * 0.42f;
    DisplayText.setPosition({(float)X, (float)Y});

    X = WindowWidth * 0.375f, Y = WindowHeight * 0.448f;
    CreateButton(Blink, BlinkTex, "Assets/Switch User/shell_editboxcursor.jpg", X, Y, 1.5, 1.5);

    X = WindowWidth * 0.5f, Y = WindowHeight * 0.58f;
    CreateButton(DoneAddingUser, NewButtonTex, "Assets/Switch User/Button.png", X, Y, 1.5, 1.5);
    SetupButtonText(DoneAddingUserText, "Done", DoneAddingUser);

    if (!CancelAddingUserHLTex.loadFromFile("Assets/Switch User/shell_tinybtn124_high.jpg"))
        cout << "Can't load Texture!\n";

    X = WindowWidth * 0.5f, Y = WindowHeight * 0.68f;
    CreateButton(CancelAddingUser, CancelAddingUserTex, "Assets/Switch User/shell_tinybtn124_normal.jpg", X, Y, 1.5, 1.5);
    SetupButtonText(CancelAddingUserText, "Cancel", CancelAddingUser);
}

void RefreshUsersList()
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

void DeleteUser()
{
    JustOpenDeleteConfirm = 1;
    isConfirmUserDelete = 1;
    unsigned int X = WindowWidth * 0.5f, Y = WindowHeight * 0.5f;
    CreateButton(DeleteUserBg, DelteUserBgTex, "Assets/Switch User/DeleteUserBg.png", X, Y, 0.25, 0.25);

    X = WindowWidth * 0.4f, Y = WindowHeight * 0.65f;
    CreateButton(YesButton, NewButtonTex, "Assets/Switch User/Button.png", X, Y, 1.5, 1.5);
    SetupButtonText(YesButtonText, "Yes", YesButton);

    X = WindowWidth * 0.6f, Y = WindowHeight * 0.65f;
    CreateButton(NoButton, NewButtonTex, "Assets/Switch User/Button.png", X, Y, 1.5, 1.5);
    SetupButtonText(NoButtonText, "No!", NoButton);

    SetupButtonText(DeletethisUser, users[SelectedUser].first, DeleteUserBg);
    X = WindowWidth * 0.49f, Y = WindowHeight * 0.53f;
    DeletethisUser.setPosition({(float)X, (float)Y});
    DeletethisUser.setCharacterSize(60);
}

void FullList()
{
    isListFull = 1;

    unsigned int X = WindowWidth * 0.5f, Y = WindowHeight * 0.5f;
    CreateButton(ListisFull, ListisFullTex, "Assets/Switch User/ListIsFull.png", X, Y, 0.25, 0.25);

    X = WindowWidth * 0.5f, Y = WindowHeight * 0.64f;
    CreateButton(FullOKButton, FullOKButtonTex, "Assets/Switch User/Button.png", X, Y, 1.5, 1.5);
    SetupButtonText(OKText, "OK", FullOKButton);

    InputString = "";
}

void DupplicateUser()
{
    DupplicateName = 1;
    unsigned int X = WindowWidth * 0.5f, Y = WindowHeight * 0.5f;
    CreateButton(DupplicateBg, DupplicateBgTex, "Assets/Switch User/DupplicateUser.png", X, Y, 0.25, 0.25);

    X = WindowWidth * 0.5f, Y = WindowHeight * 0.64f;
    CreateButton(DupplicateOKButton, DupplicateOKButtonTex, "Assets/Switch User/Button.png", X, Y, 1.5, 1.5);
    SetupButtonText(OKText, "OK", DupplicateOKButton);

    SetupButtonText(DupplicatedUserText, InputString, DupplicateBg);
    X = WindowWidth * 0.49f, Y = WindowHeight * 0.53f;
    DupplicatedUserText.setPosition({(float)X, (float)Y});
    DupplicatedUserText.setCharacterSize(60);
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
        if (UserTexts[user] != nullptr)
            window.draw(*UserTexts[user]);

    if (SelectedUser >= 0)
        window.draw(SelectUserHL);

    if (isUserSelected)
    {
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
        window.draw(OKText);
    }
    if (DupplicateName and !isListFull and !isConfirmUserDelete and !NameEntry and !isUserSelected)
    {
        DrawMainMenuBackground();
        window.draw(DupplicateBg);
        window.draw(DupplicateOKButton);
        window.draw(OKText);
        window.draw(DupplicatedUserText);
    }
    window.display();
}