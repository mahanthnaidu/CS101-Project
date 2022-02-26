#include <simplecpp>
#include "shooter.h"
#include "bubble.h"
#include<cmath>

/* Simulation Vars */
const double STEP_TIME = 0.02;

/* Game Vars */
const int PLAY_Y_HEIGHT = 450;
const int LEFT_MARGIN = 70;
const int TOP_MARGIN = 20;
const int BOTTOM_MARGIN = (PLAY_Y_HEIGHT+TOP_MARGIN);


void move_bullets(vector<Bullet> &bullets){
    // move all bullets
    for(unsigned int i=0; i<bullets.size(); i++){
        if(!bullets[i].nextStep(STEP_TIME)){
            bullets.erase(bullets.begin()+i);
        }
    }
    return ; // added .
}

void move_bubbles(vector<Bubble> &bubbles){
    // move all bubbles
    for (unsigned int i=0; i < bubbles.size(); i++)
    {
        bubbles[i].nextStep(STEP_TIME);
    }
    return ;  // added .
}

vector<Bubble> create_bubbles()
{
    // create initial bubbles in the game
    vector<Bubble> bubbles;
    bubbles.push_back(Bubble(2*WINDOW_X/7.0,BUBBLE_START_Y,20, -BUBBLE_DEFAULT_VX, 50, COLOR(255,105,0)));  // changed .
    bubbles.push_back(Bubble(3*WINDOW_X/7.0,50 + BUBBLE_START_Y, 10, -BUBBLE_DEFAULT_VX, 75, COLOR(255,105,180)));  // changed .
    bubbles.push_back(Bubble(4*WINDOW_X/7.0, 50 + BUBBLE_START_Y, 10,-BUBBLE_DEFAULT_VX ,50, COLOR(255,105,180)));  // changed .
    bubbles.push_back(Bubble(5*WINDOW_X/7.0, 50 + BUBBLE_START_Y, 10, BUBBLE_DEFAULT_VX, 75, COLOR(255,105,180)));  // changed .
    bubbles.push_back(Bubble(6*WINDOW_X/7.0, BUBBLE_START_Y, 20,BUBBLE_DEFAULT_VX, 50, COLOR(255,105,0)));  // changed .
    return bubbles;
}

int main()
{
    initCanvas("                                                 Bubble Trouble", WINDOW_X, WINDOW_Y);  // Bubble Shooter title must be in the middle .

    Line b1(0, PLAY_Y_HEIGHT, WINDOW_X, PLAY_Y_HEIGHT);
    b1.setColor(COLOR(0, 0, 255));

    string msg_cmd("Cmd: _");
    Text charPressed(LEFT_MARGIN, BOTTOM_MARGIN, msg_cmd);

    // Intialize the shooter
    Shooter shooter(SHOOTER_START_X, SHOOTER_START_Y, SHOOTER_VX);

    // Initialize the bubbles
    vector<Bubble> bubbles = create_bubbles();

    // Initialize the bullets (empty)
    vector<Bullet> bullets;

    int Bubbles_Burst=0 , Health=5, Time=1 ;   // Bubbles_Burst represents number of bubbles of radius 10 which were burst .

    string mssg_time("Time : ");  // Text reprsenting Time on canavs window .
    Text Time_(30,20,mssg_time);

    string total_time("/ 60");  // Text reprsenting Total Time on canavs window .
    Text Total_time(83,20,total_time);

    string mssg_health("Health : ");
    Text Health_(430,20,mssg_health);  // Text reprsenting Health on canavs window .

    string init_health("/ 5");
    Text Init_Health(480,20,init_health);  // Text reprsenting Initial Health on canavs window .

    string mssg_score("Score : ");
    Text Score_(450-LEFT_MARGIN,BOTTOM_MARGIN,mssg_score);  // Text reprsenting score on canavs window .

    string max_score("/ 7");
    Text Max_score(425,BOTTOM_MARGIN,max_score);  // Text reprsenting maximum score on canavs window .

    Text TIME(60,20,Time/18);  // variable representing Time ( actually it's speed is 18 times faster than the seconds )
    Text HEALTH(465,20,Health);  // variable representing Health of the shooter .
    Text SCORE(420,470,Bubbles_Burst);  // variable representing score .

    XEvent event;

    // Main game loop
    while (true)
    {
        bool pendingEvent = checkEvent(event);
        if (pendingEvent)
        {
            // Get the key pressed
            char c = charFromEvent(event);
            msg_cmd[msg_cmd.length() - 1] = c;
            charPressed.setMessage(msg_cmd);

            // Update the shooter
            if(c == 'a')
                shooter.move(STEP_TIME, true);
            else if(c == 'd')
                shooter.move(STEP_TIME, false);
            else if(c == 'w')
                bullets.push_back(shooter.shoot());
            else if(c == 'q')
                return 0;
        }

        // Update the bubbles
        move_bubbles(bubbles);

        // Update the bullets
        move_bullets(bullets);

        bool Big_Burst = false ;  // bool value which will be true when a big bubble( of radius 20 ) was burst .

        double small_x1,small_x2,small_y1,small_y2;  // coordinates of the two smaller bubbles formed .

        for (unsigned int i=0; i < bubbles.size(); i++)

    {    for(unsigned int j=0; j<bullets.size(); j++){

        int a = (bubbles[i].get_center_x()-bullets[j].get_center_x())*(bubbles[i].get_center_x()-bullets[j].get_center_x());

        int b = (bubbles[i].get_center_y()-bullets[j].get_center_y())*(bubbles[i].get_center_y()-bullets[j].get_center_y());

        int c = (bubbles[i].get_radius()+(bullets[j].get_height()/sqrt(2)))*(bubbles[i].get_radius()+(bullets[j].get_height()/sqrt(2)));

        if(a+b<c){  // condition for bullet striking the bubble .
            if(bubbles[i].get_radius()==10){  // if the  bullet strikes the smaller bubble .

                Bubbles_Burst = Bubbles_Burst+1 ;

                bubbles.erase(bubbles.begin()+i);  // smaller bubble disappearing .

                bullets.erase(bullets.begin()+j) ;  // bullet also disappearing after striking .

                Big_Burst = false ;

                break ;
            }
            else if(bubbles[i].get_radius()==20){  // if the bullet strikes the bigger bubble .

                small_x1 = bubbles[i].get_center_x() + 10;  // x cordinate of the one of the resultant small bubble .

                small_x2 = bubbles[i].get_center_x() - 10;  // x cordinate of the another resultant small bubble .

                small_y1 = bubbles[i].get_center_y() ;  // y cordinate of the one of the resultant small bubble .

                small_y2 = bubbles[i].get_center_y() ;  // x cordinate of the one of the resultant small bubble .

                bubbles.erase(bubbles.begin()+i);  // bigger bubble disappearing .

                bullets.erase(bullets.begin()+j) ;  // bullet also disappearing .

                Big_Burst = true ;

                break ;

            }

        }

    }

    }
    if(Big_Burst){

        bubbles.push_back(Bubble(small_x1,small_y1, BUBBLE_RADIUS_THRESHOLD,-BUBBLE_DEFAULT_VX, BUBBLE_DEFAULT_VY, COLOR(255,105,180)));
                // creating new small bubble .
        bubbles.push_back(Bubble(small_x2,small_y2, BUBBLE_RADIUS_THRESHOLD,BUBBLE_DEFAULT_VX, BUBBLE_DEFAULT_VY, COLOR(255,105,180)));
                 // creating new small bubble .
    }
    {
        wait(STEP_TIME);
        Time++;
        TIME.reset(60,20,Time/18);  // changing the value of variable Time second by second .
    }
    SCORE.reset(410,470,Bubbles_Burst);
    for (unsigned int i=0; i < bubbles.size(); i++){
        if((abs(bubbles[i].get_center_x() - shooter.get_body_center_x()) < (shooter.get_body_width()/2.0+bubbles[i].get_radius())&&(450 - bubbles[i].get_center_y() < 2*shooter.get_head_radius()+shooter.get_body_height()+bubbles[i].get_radius())
        )) // condition for the bubble to hit the shooter .
        {
            Health = Health - 1 ;  // health reduces by 1 when a bubble hits the shooter .

            shooter.change_position();
             // changing the position of the shooter if a bubble hits it .
             if(Health<1){

             break ;

             }

    }
    }
    HEALTH.reset(465,20,Health);
    if(Bubbles_Burst==7){   // total  number of bubbles burst = 7 for winning the game .
        for(unsigned int p=0;p<(bubbles.size());p++){
            bubbles.erase(bubbles.begin()+p);  //  erasing all when the Game is over .
        }
        for(unsigned int m=0;m<(bullets.size());m++){
            bullets.erase(bullets.begin()+m);  //  erasing all when the Game is over .
        }
        Text Winning_The_Game(250,225,"CONGRATULATIONS !! YOU HAVE WON THE GAME :) : )");  // Output to be appeared if total number of Bubbles burst becomes seven .
        wait(20);
        break ;
    }
    else if(Time==1080){  // 1080 = 18*60 ; because total time is max 60 sec ( actual time is 18 times the rate of the updating variable Time ) .
        for(unsigned int p=0;p<(bubbles.size());p++){  // erasing all when the Game is over .
            bubbles.erase(bubbles.begin()+p);
        }
        for(unsigned int m=0;m<(bullets.size());m++){
            bullets.erase(bullets.begin()+m);  //  erasing all when the Game is over .
        }

        Text TIME_UP(225,225,"SORRY , YOUR TIME IS OVER  :(");  // Output to be appeared if total time become 60 sec .
        wait(20);
        break ;
    }
    else if(Health==0){  // health should be positive for the game to be continued
        for(unsigned int p=0;p<(bubbles.size());p++){
            bubbles.erase(bubbles.begin()+p);  //  erasing all when the Game is over .
        }
        for(unsigned int m=0;m<(bullets.size());m++){ //  erasing all when the Game is over .
            bullets.erase(bullets.begin()+m);
        }

        Text Loosing_The_Game(225,225,"SORRY , YOU HAVE LOST YOUR GAME :(");  // Output to be appeared if Health becomes zero .
        wait(20);
        break ;
    }

    }
}
