#include <sstream>
#include <bits/stdc++.h>
#include <SFML/Graphics.hpp>
#include<SFML/Audio.hpp>

const int Num_branch = 6;
sf::Sprite branch[Num_branch];
enum class side
{
    LEFT,
    RIGHT,
    NONE
};
side branchPosition[Num_branch];

void updateBranches(int seed)
{
    for (int j = Num_branch - 1; j > 0; j--)
    {
        branchPosition[j] = branchPosition[j - 1];
    }
    srand((int)time(0) % 5 + seed);
    int r = (rand() % 5);
    switch (r)
    {
    case 0:
        branchPosition[0] = side::LEFT;
        break;

    case 1:
        branchPosition[0] = side::RIGHT;
        break;

    default:
        branchPosition[0] = side::NONE;
        break;
    }
}

int main()
{
    srand((int)time(0));
    int number = (rand() % 100);
    sf::VideoMode vm(1920, 1080);
    sf::RenderWindow window(vm, "Tinder Man");
    window.setFramerateLimit(960);

    //BackGround
    sf::Texture textureBackGround;
    textureBackGround.loadFromFile("src/background.png");
    sf::Sprite spriteBackground;
    spriteBackground.setTexture(textureBackGround);

    //player
    sf::Texture texturePlayer;
    texturePlayer.loadFromFile("src/timberman.png");
    sf::Sprite spritePlayer;
    spritePlayer.setTexture(texturePlayer);
    spritePlayer.setPosition(580, 720);
    side playerSide = side::LEFT;
    
    
    //prepare the axe
    sf::Texture textureRip;
    textureRip.loadFromFile("src/rip.png");
    sf::Sprite spriteRip;
    spriteRip.setTexture(textureRip);
    spriteRip.setPosition(600, 860);

    //prepare the axe
    sf::Texture textureAxe;
    textureAxe.loadFromFile("src/axe.png");
    sf::Sprite spriteAxe;
    spriteAxe.setTexture(textureAxe);
    spriteAxe.setPosition(700, 830);

    //line the axe up with the tree
    const float AXE_POSITION_LEFT = 700;
    const float AXE_POSITION_RIGHT = 1075;

    //Prepare the flying log

    sf::Texture textureLog;
    textureLog.loadFromFile("src/log.png");
    sf::Sprite spriteLog;
    spriteLog.setTexture(textureLog);
    spriteLog.setPosition(800, 720);

    //other log related variable
    bool logActive = false;
    float logSpeedX = 1000;
    float logSpeedY = -1500;

    //Font
    sf::Font font;
    font.loadFromFile("src/fonts.ttf");

    //Text
    sf::Text text;
    text.setString("Please Enter to Start");
    text.setCharacterSize(36);
    text.setFillColor(sf::Color::White);
    text.setFont(font);

    //Tree
    sf::Texture textureTree;
    textureTree.loadFromFile("src/tree.png");
    sf::Sprite spriteTree;
    spriteTree.setTexture(textureTree);
    spriteTree.setScale(1, 1);
    spriteTree.setPosition(800, 0);

    //bee
    sf::Texture textureBee;
    textureBee.loadFromFile("src/bee.png");
    sf::Sprite spriteBee;
    spriteBee.setTexture(textureBee);
    spriteBee.setPosition(0, 800);
    bool beeActive = false;
    float beeSpeed = 0.0f;

    //clouds
    sf::Texture textureCloud;
    textureCloud.loadFromFile("src/cloud.png");
    sf::Sprite spriteCloud[3];
    bool cloudActive[3];
    float cloudSpeed[3];
    for (int i = 0; i < 3; i++)
    {
        spriteCloud[i].setTexture(textureCloud);
        spriteCloud[i].setPosition(0, (i + 1) * 120);
        cloudActive[i] = false;
        cloudSpeed[i] = 0.0f;
    }

    sf::Clock clock;

    //time bar
    sf::RectangleShape timeBar;
    float timeBarStartWidth = 1920;
    float timeBarStartHeight = 5;
    timeBar.setSize(sf::Vector2f(timeBarStartWidth, timeBarStartHeight));
    timeBar.setFillColor(sf::Color::Red);
    timeBar.setOutlineColor(sf::Color::White);

    //timeBar.setPosition((1920/2.0f)-timeBarStartWidth/2.0f,980);
    timeBar.setPosition(0, 0);
    sf::Time getTimeTotal;
    float timeRemaining = 6.0f;
    float timeBarWidthPerSec = timeBarStartWidth / timeRemaining;

    bool paused = true;

    //Drawing Text
    int score = 0;
    sf::Text msgText;
    sf::Text scoreText;
    msgText.setFont(font);
    scoreText.setFont(font);
    msgText.setCharacterSize(36);
    scoreText.setCharacterSize(36);
    msgText.setString("Press Enter to start!!");
    scoreText.setString("Score = 0");
    msgText.setFillColor(sf::Color::White);
    scoreText.setFillColor(sf::Color::White);

    //Prepare branches
    sf::Texture textureBranch;
    textureBranch.loadFromFile("src/branch.png");
    for (int i = 0; i < 6; i++)
    {
        branch[i].setTexture(textureBranch);
        branch[i].setPosition(-2000, -2000);
        branch[i].setOrigin(200, 20);
    }
    updateBranches(1);
    updateBranches(2);
    updateBranches(3);
    updateBranches(4);
    updateBranches(5);

    bool acceptInput = false;

    //Prepare the sound
    sf::SoundBuffer chopBuffer;
    chopBuffer.loadFromFile("sfx/chop.wav");
    sf::Sound chop;
    chop.setBuffer(chopBuffer);

    sf::SoundBuffer deathBuffer;
    deathBuffer.loadFromFile("sfx/death.wav");
    sf::Sound death;
    death.setBuffer(deathBuffer);

    sf::SoundBuffer timeoutBuffer;
    timeoutBuffer.loadFromFile("sfx/timeout.wav");
    sf::Sound timeout;
    timeout.setBuffer(timeoutBuffer);


    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::EventType::Closed)
            {
                window.close();
                break;
            }
            if (event.type == sf::Event::KeyReleased && !paused)
            {
                acceptInput = true;
                spriteAxe.setPosition(2000, spriteAxe.getPosition().y);
            }
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
        {
            paused = false;
            score = 0;
            timeRemaining = 6;
            for (int i = 1; i < Num_branch; i++)
                branchPosition[i] = side::NONE;
            spriteRip.setPosition(675, 2000);
            spritePlayer.setPosition(580, 720);
            acceptInput = true;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::BackSpace))
            paused = true;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
            window.close();

        //wrap the player control to
        //make sure we are accepting input from keyboard
        if (acceptInput)
        {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
            {
                playerSide = side::RIGHT;
                score++;
                //add time
                timeRemaining += (2 / score) + 0.15;
                spriteAxe.setPosition(AXE_POSITION_RIGHT, spriteAxe.getPosition().y);
                spritePlayer.setPosition(1200, 720);
                updateBranches(score);
                spriteLog.setPosition(800, 720);
                logSpeedX = -5000;
                logActive = true;
                acceptInput = false;
                chop.play();
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
            {
                playerSide = side::LEFT;
                score++;
                //add time
                timeRemaining += (2 / score) + 0.15;
                spriteAxe.setPosition(AXE_POSITION_LEFT, spriteAxe.getPosition().y);
                spritePlayer.setPosition(580, 720);
                updateBranches(score);
                spriteLog.setPosition(800, 720);
                logSpeedX = 5000;
                logActive = true;
                acceptInput = false;
                chop.play();
            }
        }
        //Position the text
        sf::FloatRect textRect = msgText.getLocalBounds();
        msgText.setOrigin(textRect.left + textRect.width / 2.0f,
                          textRect.top + textRect.height / 2.0f);
        msgText.setPosition(1920 / 2.0f, 1080 / 2.0f);
        scoreText.setPosition(20, 20);

        //update scene
        if (!paused)
        {
            sf::Time dt = clock.restart();
            timeRemaining -= dt.asSeconds();
            timeBar.setSize(sf::Vector2f(timeBarWidthPerSec * timeRemaining, timeBarStartHeight));

            if (timeRemaining < 0.0f)
            {
                paused = true;
                msgText.setString("Time End");
                sf::FloatRect t_rect = msgText.getLocalBounds();
                msgText.setOrigin(t_rect.left + t_rect.width / 2.0f,
                                  t_rect.top + t_rect.height / 2.0f);
                msgText.setPosition(1920 / 2.0f, 1080 / 2.0f);
            }

            //set up the bee
            if (!beeActive)
            {
                srand((int)time(0));
                beeSpeed = (rand() % 200) + 200;

                //height
                srand((int)time(0) * 10);
                float height = (rand() % 500) + 400;
                spriteBee.setPosition(2000, height);
                beeActive = true;
            }
            else
            {
                spriteBee.setPosition(spriteBee.getPosition().x - (beeSpeed * dt.asSeconds()),
                                      spriteBee.getPosition().y);
                if (spriteBee.getPosition().x < -100)
                    beeActive = false;
            }
            int t = 0;
            //setting up the clouds
            for (int i = 0; i < 3; i++)
            {
                t = i + 1;
                if (!cloudActive[i])
                {
                    srand((int)time(0) * 10 * i);
                    cloudSpeed[i] = (rand() % 200) + 100 * t;

                    srand((int)time(0) * 10 * t);
                    float height = (rand() % (150 * t)) - 150 * i;
                    if (spriteCloud[i].getPosition().y < 20)
                        height = 20;
                    spriteCloud[i].setPosition(-200, height);
                    cloudActive[i] = true;
                }
                else
                {
                    spriteCloud[i].setPosition(spriteCloud[i].getPosition().x + (cloudSpeed[i] * dt.asSeconds()),
                                               spriteCloud[i].getPosition().y);
                    if (spriteCloud[i].getPosition().x > 1920)
                        cloudActive[i] = false;
                }
            }
            std::stringstream ss;
            ss << "Score = " << score;
            scoreText.setString(ss.str());

            for (int i = 0; i < Num_branch; i++)
            {
                float height = i * 150;
                if (branchPosition[i] == side::LEFT)
                {
                    branch[i].setPosition(600, height);
                    branch[i].setRotation(180);
                }
                else if (branchPosition[i] == side::RIGHT)
                {
                    branch[i].setPosition(1300, height);
                    branch[i].setRotation(0);
                }
                else
                {
                    branch->setPosition(3000, height);
                }
            }
            //Handle Flying Log
            if(logActive){
                spriteLog.setPosition(spriteLog.getPosition().x + (logSpeedX*dt.asSeconds()),
                spriteLog.getPosition().y+(logSpeedY*dt.asSeconds()));
                if(spriteLog.getPosition().x>2000 || spriteLog.getPosition().x<-100){
                    logActive = false;
                    spriteLog.setPosition(800, 720);
                }
            }
            //hit by branch??
            if(branchPosition[5]==playerSide){
                paused = true;
                acceptInput = false;
                //Draw the Rip
                spriteRip.setPosition(525,760);
                //Hide the player
                spritePlayer.setPosition(2000,660);
                msgText.setString("Squashed!!!");
                sf::FloatRect textRCT = msgText.getLocalBounds();
                msgText.setOrigin(textRCT.left + textRCT.width/2.0f,
                            textRCT.top + textRCT.height/2.0f);
                msgText.setPosition(1920/2.0f,1080/2.0f);
                //death.setPitch(0.5);
                death.play();
            }
        }

        

        //draw the scene
        //show
        window.clear();
        window.draw(spriteBackground);
        window.draw(spriteBee);
        for (int i = 0; i < Num_branch; i++)
            window.draw(branch[i]);

        for (int i = 0; i < 3; i++)
            window.draw(spriteCloud[i]);
        if (paused)
            window.draw(msgText);
        window.draw(spriteTree);
        window.draw(spritePlayer);
        window.draw(spriteAxe);
        window.draw(spriteLog);
        window.draw(spriteRip);
        window.draw(timeBar);
        window.draw(scoreText);
        window.display();
    }
    return 0;
}