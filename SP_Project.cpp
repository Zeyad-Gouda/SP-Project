#include <iostream>
#include <string>
#include <fstream>
#include <SFML/Graphics.hpp>
using namespace std;
using namespace sf;

unsigned int WindowWidth = 1920, WindowHeight = 1080;
RenderWindow window(VideoMode({WindowWidth, WindowHeight}), "Game");
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

void SwitchUser();
void Start();
void CreateSprite(Sprite &sprite, Texture &texture, const string &filePath, // A Template for creating sprites
                  float xPosition,
                  float yPositon, float scaleX, float scaleY);
void SetupText(Text &text, const string &str, Sprite &button); // A Template for Creating Text for buttons

void Update();
void HoverButton(Sprite &button, const Texture &normalTex,
                 const Texture &highlightTex, Vector2f mousePos); // A Template for Hovering Over Buttons
void EnterYourName();
void RefreshUsersList();
void DeleteUser();
void FullList();       // If the List is Full -> (max is 7 users)
void DupplicateUser(); // If the user tried to enter a name that already exists

void Display();

int main()
{
    SwitchUser(); // The Main Function
    return 0;
}

void SwitchUser()
{
    Start();
    while (window.isOpen())
    {
        Update();
        Display();
    }
}

void Start()
{
    for (int i = 0; i < 8; i++)
        UserTexts[i] = nullptr; // Initializing Users with Null

    float X = WindowWidth / 2.f, Y = WindowHeight / 2.f;
    CreateSprite(Full, FullTex, "BG.png", X, Y, 0.25, 0.25); // Background of the SwitchUser pop-up window

    X = WindowWidth * 0.37f, Y = WindowHeight * 0.68f;
    CreateSprite(NewButton, NewButtonTex, "Button.png", X, Y, 1.5, 1.5);
    if (!NewButtonHLTex.loadFromFile("Button High.png"))
        cout << "Failed to load: ButtonHL" << "\n";

    X = WindowWidth * 0.5f, Y = WindowHeight * 0.68f;
    CreateSprite(SelectButton, NewButtonTex, "Button.png", X, Y, 1.5, 1.5);
    if (!NewButtonHLTex.loadFromFile("Button High.png"))
        cout << "Failed to load: ButtonHL" << "\n";

    X = WindowWidth * 0.62f, Y = WindowHeight * 0.68f;
    CreateSprite(DeleteButton, NewButtonTex, "Button.png", X, Y, 1.5, 1.5);
    if (!NewButtonHLTex.loadFromFile("Button High.png"))
        cout << "Failed to load: ButtonHL" << "\n";

    if (!btnFont.openFromFile("BarmenoBold.otf"))
        cout << "Can't load font\n";
    SetupText(NewText, "New", NewButton);
    SetupText(SelectText, "Select", SelectButton);
    SetupText(DeleteText, "Delete", DeleteButton);

    X = WindowWidth * 0.5f, Y = WindowHeight * 0.25f;
    CreateSprite(Title, TitleTex, "shell_chooseuser_hdr.png", X, Y, 1.5, 1.5);

    // ----------- Checking to see if we already Have Users------------------
    ifstream loadusers("Users_List.txt");
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
void SetupText(Text &text, const string &str, Sprite &button)
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
void CreateSprite(Sprite &sprite, Texture &texture, const string &filePath,
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

void Update()
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
                    NameEntry = 0;
                    isCancelAddingUser = 1;
                    InputString = "";
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

                        ofstream rewrite("Users_List.txt");
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
                    if (DupplicateName and !isListFull and !isConfirmUserDelete and !NameEntry and !isUserSelected)
                        DupplicateName = 0;
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
    CreateSprite(EnterYourNamebg, EnterYourNameBgTex, "EnterYourName.png", X, Y, 0.25, 0.25);

    if (!Inputfont.openFromFile("trebuc.ttf"))
        cout << "Cant Open Font!";

    DisplayText.setFont(Inputfont);
    DisplayText.setCharacterSize(40);
    DisplayText.setFillColor(Color::White);
    X = WindowWidth * 0.38f, Y = WindowHeight * 0.40f;
    DisplayText.setPosition({(float)X, (float)Y});

    X = WindowWidth * 0.38f, Y = WindowHeight * 0.42f;
    DisplayText.setPosition({(float)X, (float)Y});

    X = WindowWidth * 0.375f, Y = WindowHeight * 0.448f;
    CreateSprite(Blink, BlinkTex, "shell_editboxcursor.jpg", X, Y, 1.5, 1.5);

    X = WindowWidth * 0.5f, Y = WindowHeight * 0.58f;
    CreateSprite(DoneAddingUser, NewButtonTex, "Button.png", X, Y, 1.5, 1.5);
    SetupText(DoneAddingUserText, "Done", DoneAddingUser);

    if (!CancelAddingUserHLTex.loadFromFile("shell_tinybtn124_high.jpg"))
        cout << "Can't load Texture!\n";

    X = WindowWidth * 0.5f, Y = WindowHeight * 0.68f;
    CreateSprite(CancelAddingUser, CancelAddingUserTex, "shell_tinybtn124_normal.jpg", X, Y, 1.5, 1.5);
    SetupText(CancelAddingUserText, "Cancel", CancelAddingUser);
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
    CreateSprite(DeleteUserBg, DelteUserBgTex, "DeleteUserBg.png", X, Y, 0.25, 0.25);

    X = WindowWidth * 0.4f, Y = WindowHeight * 0.65f;
    CreateSprite(YesButton, NewButtonTex, "Button.png", X, Y, 1.5, 1.5);
    SetupText(YesButtonText, "Yes", YesButton);

    X = WindowWidth * 0.6f, Y = WindowHeight * 0.65f;
    CreateSprite(NoButton, NewButtonTex, "Button.png", X, Y, 1.5, 1.5);
    SetupText(NoButtonText, "No!", NoButton);

    SetupText(DeletethisUser, users[SelectedUser].first, DeleteUserBg);
    X = WindowWidth * 0.49f, Y = WindowHeight * 0.53f;
    DeletethisUser.setPosition({(float)X, (float)Y});
    DeletethisUser.setCharacterSize(60);
}

void FullList()
{
    isListFull = 1;

    unsigned int X = WindowWidth * 0.5f, Y = WindowHeight * 0.5f;
    CreateSprite(ListisFull, ListisFullTex, "ListIsFull.png", X, Y, 0.25, 0.25);

    X = WindowWidth * 0.5f, Y = WindowHeight * 0.64f;
    CreateSprite(FullOKButton, FullOKButtonTex, "Button.png", X, Y, 1.5, 1.5);
    SetupText(OKText, "OK", FullOKButton);

    InputString = "";
}

void DupplicateUser()
{

    DupplicateName = 1;
    unsigned int X = WindowWidth * 0.5f, Y = WindowHeight * 0.5f;
    CreateSprite(DupplicateBg, DupplicateBgTex, "DupplicateUser.png", X, Y, 0.25, 0.25);

    X = WindowWidth * 0.5f, Y = WindowHeight * 0.64f;
    CreateSprite(DupplicateOKButton, DupplicateOKButtonTex, "Button.png", X, Y, 1.5, 1.5);
    SetupText(OKText, "OK", DupplicateOKButton);

    SetupText(DupplicatedUserText, InputString, DupplicateBg);
    X = WindowWidth * 0.49f, Y = WindowHeight * 0.53f;
    DupplicatedUserText.setPosition({(float)X, (float)Y});
    DupplicatedUserText.setCharacterSize(60);
}

void Display()
{
    window.clear(Color(135, 206, 235));
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
    {
        window.close();
        // Get Back to the Main Menu
    }

    if (NameEntry and !isConfirmUserDelete and !isListFull and !DupplicateName)
    {
        window.clear(Color(100, 100, 100));
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
        window.clear(Color(100, 100, 100));
        window.draw(DeleteUserBg);
        window.draw(YesButton);
        window.draw(NoButton);
        window.draw(YesButtonText);
        window.draw(NoButtonText);
        window.draw(DeletethisUser);
    }
    if (isListFull and !isConfirmUserDelete and !NameEntry and !DupplicateName and !isUserSelected)
    {
        window.clear(Color(100, 100, 100));
        window.draw(ListisFull);
        window.draw(FullOKButton);
        window.draw(OKText);
    }
    if (DupplicateName and !isListFull and !isConfirmUserDelete and !NameEntry and !isUserSelected)
    {
        window.clear(Color(100, 100, 100));
        window.draw(DupplicateBg);
        window.draw(DupplicateOKButton);
        window.draw(OKText);
        window.draw(DupplicatedUserText);
    }
    window.display();
}