/* *****************************************************************************
 * A.L.E (Arcade Learning Environment)
 * Copyright (c) 2009-2013 by Yavar Naddaf, Joel Veness, Marc G. Bellemare,
 *  Matthew Hausknecht, and the Reinforcement Learning and Artificial Intelligence 
 *  Laboratory
 * Released under the GNU General Public License; see License.txt for details. 
 *
 * Based on: Stella  --  "An Atari 2600 VCS Emulator"
 * Copyright (c) 1995-2007 by Bradford W. Mott and the Stella team
 *
 * *****************************************************************************
 *  videoRecordingExample.cpp 
 *
 *  An example on recording video with the ALE. This requires SDL. 
 *  See manual for details. 
 **************************************************************************** */

#include <iostream>
#include <ale_interface.hpp>
#include <cstdlib>
#include <math.h>

#define PADDLE_MIN 27450
#define PADDLE_MAX 790196 

#ifndef __USE_SDL
#error Video recording example is disabled as it requires SDL. Recompile with -DUSE_SDL=ON. 
#else

#include <SDL.h>

using namespace std;

int main(int argc, char** argv) {
    ALEInterface ale;

    // Get & Set the desired settings
    ale.setInt("random_seed", 123);

    // We enable both screen and sound, which we will need for recording. 
    //ale.setBool("display_screen", true);
    // You may leave sound disabled (by setting this flag to false) if so desired. 
    ale.setBool("sound", false);

    std::string recordPath = "record";
    std::cout << std::endl;

    // Set record flags
    //ale.setString("record_screen_dir", recordPath.c_str());
    //ale.setString("record_sound_filename", (recordPath + "/sound.wav").c_str());
    // We set fragsize to 64 to ensure proper sound sync 
    //ale.setInt("fragsize", 64);
    ale.setInt("frame_skip", 1);
    ale.setFloat("repeat_action_probability", -100);

    // Not completely portable, but will work in most cases
    std::string cmd = "mkdir ";
    cmd += recordPath; 
    system(cmd.c_str());

    // Load the ROM file. (Also resets the system for new settings to
    // take effect.)
    ale.loadROM("kaboom.bin");

    // Get the vector of legal actions
    ActionVect legal_actions = ale.getLegalActionSet();

    std::vector<unsigned char> screen;
    screen.reserve(210*160);

    Action LEFT = (Action)12;
    Action RIGHT = (Action)11;
    Action NOOP = (Action)0;
    Action FIRE = (Action)1;
    Action FAST_LEFT = (Action)13;
    Action FAST_RIGHT = (Action)10;    

    int bucketX = 0;
    int fakeX = 0;
    int previousBucket = 0;
    Action previousAction = (Action)0;
    Action previousNonzeroAction = (Action)0;    
    int waitToMove = 0;

    int bucketDelta = 0;
    int moveDelta = 0;
    int score = 0;

    int r = PADDLE_MIN;
    int movement ;

    // Play a single episode, which we record. 
    while (!ale.game_over()) {
      
      ale.getScreenGrayscale(screen);

      int lookAt = 187;

      int bombX = 0;
      int bombY = 0;
      
      while( bombX == 0 ) {
        lookAt --;
        if (lookAt == 50) break;
        //if (lookAt == 171) lookAt = 163;
        //if (lookAt == 155) lookAt = 147;
        
        int bombTotals = 0;
        int bombCount = 0;
        int group = 0;
        for( int x = 0; x < 160; x++ ) {
          if (screen[x + lookAt * 160] == 0)
            continue;

          if (screen[x + lookAt * 160] == 0xAA) {
            group++;

            if (screen[x + 1 + lookAt * 160] == 0xAA)
              group--;

            bombTotals += x;
            bombCount++;
          }
        }        
        
        if (bombCount > 0) {
          bombX = bombTotals / bombCount;
          bombY = lookAt;
        }

      }

      lookAt -= 3;
      
      int nextBombX = 0;
      int nextBombY = 0;            
      
      while( nextBombX == 0 ) {
        lookAt --;
        if (lookAt <= 50) break;
        
        int bombTotals = 0;
        int bombCount = 0;
        int group = 0;
        for( int x = 0; x < 160; x++ ) {
          if (screen[x + lookAt * 160] == 0)
            continue;

          if (screen[x + lookAt * 160] == 0xAA) {
            group++;

            if (screen[x + 1 + lookAt * 160] == 0xAA)
              group--;

            bombTotals += x;
            bombCount++;
          }
        }        
        
        if (bombCount > 0) {
          nextBombX = bombTotals / bombCount;
          nextBombY = lookAt;
        }

      }

      lookAt -= 3;

      int thirdBombX = 0;
      int thirdBombY = 0;            
      
      while( thirdBombX == 0 ) {
        lookAt --;
        if (lookAt <= 50) break;
        
        int bombTotals = 0;
        int bombCount = 0;
        int group = 0;
        for( int x = 0; x < 160; x++ ) {
          if (screen[x + lookAt * 160] == 0)
            continue;

          if (screen[x + lookAt * 160] == 0xAA) {
            group++;

            if (screen[x + 1 + lookAt * 160] == 0xAA)
              group--;

            bombTotals += x;
            bombCount++;
          }
        }        
        
        if (bombCount > 0) {
          thirdBombX = bombTotals / bombCount;
          thirdBombY = lookAt;
        }

      }
      
      lookAt -= 3;

      int fourthBombX = 0;
      int fourthBombY = 0;            
      
      while( fourthBombX == 0 ) {
        lookAt --;
        if (lookAt <= 50) break;
        
        int bombTotals = 0;
        int bombCount = 0;
        int group = 0;
        for( int x = 0; x < 160; x++ ) {
          if (screen[x + lookAt * 160] == 0)
            continue;

          if (screen[x + lookAt * 160] == 0xAA) {
            group++;

            if (screen[x + 1 + lookAt * 160] == 0xAA)
              group--;

            bombTotals += x;
            bombCount++;
          }
        }        
        
        if (bombCount > 0) {
          fourthBombX = bombTotals / bombCount;
          fourthBombY = lookAt;
        }

      }

      
      lookAt = 148;

      int bucketTotals = 0;
      int bucketWidth = 0;

      for( lookAt = 148; lookAt <= 150; lookAt++ ) {
        for( int x = 0; x < 160; x++ ) {
          if (screen[x + lookAt * 160] == 0)
            continue;        
          
          if (screen[x + lookAt * 160] == 148) {
            bucketTotals += x;
            bucketWidth++;
          }
        }
      }

      if (bucketWidth > 0) {
        previousBucket = bucketX;
        bucketX = bucketTotals / bucketWidth;
        moveDelta += bucketX - previousBucket;        
      }

      Action a = (Action)0;

      /*
      if (abs(bucketX - bombX) == 0)
        a = (Action)0;
      else {
        if (bucketX < bombX)
          a = (Action)1;      

        if (bucketX > bombX)
          a = (Action)(-1);
          }*/

      int goalX = bombX;

      bucketWidth = 9;
      
      int sign = 0;
      
      if (goalX < bucketX) {
        sign = -1;
      }
      
      if (goalX > bucketX) {
        sign = +1;
      }
      
      if (nextBombX != 0) {
        if (nextBombX < bombX)
          goalX = bombX - bucketWidth / 2 + 1;

        if (nextBombX > bombX)
          goalX = bombX + bucketWidth / 2 - 1;

        int average = (nextBombX + bombX)/2;
        if ((abs(average - nextBombX) < bucketWidth/2) && (abs(average - bombX) < bucketWidth/2)) {
          goalX = average;
        }
        
        if (abs(nextBombX - bombX) < bucketWidth/2) {
          goalX = nextBombX;
        }
      }
      /*
      if (thirdBombX != 0) {
        if ((abs(thirdBombX - nextBombX) < bucketWidth/2) && (abs(thirdBombX - bombX) < bucketWidth/2)) {
          goalX = thirdBombX;
        }
      }

      if (fourthBombX != 0) {
        printf("FOURTH\n");
        if ((abs(fourthBombX - nextBombX) < bucketWidth/2) && (abs(fourthBombX - bombX) < bucketWidth/2) && (abs(fourthBombX - thirdBombX) < bucketWidth/2)) {
          goalX = fourthBombX;
        }
      }
      */
      
      if (bombX == 0) {
        ale.act((Action)movement);
        continue;
      }

      //movement = sign * pow(abs(goalX - bucketX), 1.1) * 1300;
      //movement = sign * pow(abs(goalX - bucketX), 1.0) * 2300;
      //movement = sign * pow(abs(goalX - bucketX), 1.0) * 2300;
      
      //if (abs(bombX - bucketX) < bucketWidth/2)
      //movement = 1;

      //movement = PADDLE_MAX - (bombX - 7) * (PADDLE_MAX - PADDLE_MIN) / (160 - 16);

      //movement = 838287 - 4517 * bombX;
      //movement = 782964 - 4382  * bombX;
      movement = 778396 - 4331 * (bombX + bucketWidth/2);      

      /*
      printf( "### %d %d\n", r, bucketX );
      
      movement = r;
      r = r + 23000;
      if (r >= PADDLE_MAX)
        break;
      */
      
      a = (Action) ( movement ) ;
      
      //if (a == 0)
      //a = (Action) ((rand()%3) - 1);

      /*
      if (bombY >= 178) {
        printf( "****************************************************************BOOM***\n" );
        }*/
      
      //printf( "Bucket = %d (before %d) ; bomb = (%d,%d) then (%d,%d) then (%d,%d) then (%d,%d) so goal = %d; moving %d\n", bucketX, previousBucket, bombX, bombY,  nextBombX, nextBombY, thirdBombX, thirdBombY, fourthBombX, fourthBombY, goalX, (int)a );

      if (bucketX == 24)
        bucketDelta = 0;
      bucketDelta = bucketDelta + a;

      ale.act(a);                        
      //ale.act((Action)1);
      
      if (a != (Action)0) {
        previousNonzeroAction = a;        
      }
      previousAction = a;
      
      const ALERAM &ram = ale.getRAM();
      
      /*
      for( int i=0; i<128; i++ ) {
        byte_t b = ram.get(i);
        printf("%02X ", b );
      }
      printf( "\n" );
      */
      
      char scoreString[6];
      sprintf( scoreString, "%02X%02X%02X", ram.get(35), ram.get(36), ram.get(37) );
      score = atoi(scoreString);
      printf( "SCORE: %d on frame %d\n", score, ale.getFrameNumber() );

      /*
      int buckets = ram.get(33);
      if (buckets == 2) {
        break;
      }
      */
    }

    std::cout << std::endl;
    std::cout << "Recording complete. To create a video, you may want to run \n"
        "  doc/scripts/videoRecordingExampleJoinXXX.sh. See manual for details.." << std::endl;

    return 0;
}
#endif // __USE_SDL
