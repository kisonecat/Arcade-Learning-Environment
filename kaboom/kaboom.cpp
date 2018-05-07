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
    ale.setBool("display_screen", true);
    // You may leave sound disabled (by setting this flag to false) if so desired. 
    ale.setBool("sound", true);

    std::string recordPath = "record";
    std::cout << std::endl;

    // Set record flags
    ale.setString("record_screen_dir", recordPath.c_str());
    ale.setString("record_sound_filename", (recordPath + "/sound.wav").c_str());
    // We set fragsize to 64 to ensure proper sound sync 
    ale.setInt("fragsize", 64);
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

    int bucketX = 0;

    int score = 0;

    int r = PADDLE_MIN;
    int movement;

    // Play a single episode, which we record. 
    while (!ale.game_over()) {
      
      ale.getScreenGrayscale(screen);

      int lookAt = 187;

      int bombX = 0;
      int bombY = 0;
      
      while( bombX == 0 ) {
        lookAt --;
        if (lookAt == 50) break;
        
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

      int bucketWidth = 0;

      /*
      lookAt = 148;

      int bucketTotals = 0;
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
        bucketX = bucketTotals / bucketWidth;
      }
      */
      
      Action a = (Action)0;

      bucketWidth = 9;
      
      if (bombX == 0) {
        ale.act((Action)movement);
        continue;
      }

      movement = 778396 - 4331 * (bombX + bucketWidth/2);      

      /*
      printf( "### %d %d\n", r, bucketX );
      
      movement = r;
      r = r + 23000;
      if (r >= PADDLE_MAX)
        break;
      */
      
      a = (Action) ( movement ) ;
            
      //printf( "Bucket = %d (before %d) ; bomb = (%d,%d) then (%d,%d) then (%d,%d) then (%d,%d) so goal = %d; moving %d\n", bucketX, previousBucket, bombX, bombY,  nextBombX, nextBombY, thirdBombX, thirdBombY, fourthBombX, fourthBombY, goalX, (int)a );

      ale.act(a);                        
      
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

      if (score == 3000) break;
      
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
