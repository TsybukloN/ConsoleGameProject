#ifndef MAPS_H
#define MAPS_H

namespace Maps {
    // Necessary initialization area values
    const int infolineCount = 1;
    const int addCol = 3;
    const int kScreenWidth = 50 + addCol;
    const int kScreenHeight = 20 + infolineCount;

    const char symbolHeal = '+';
    const char emptyCell = ' ';

    void Init(char(&inputField)[kScreenHeight][kScreenWidth], const char outputField[kScreenHeight][kScreenWidth]) {
        for (int i = 0; i < kScreenHeight; ++i) {
            for (int j = 0; j < kScreenWidth; ++j) {
                inputField[i][j] = outputField[i][j];
            }
        }
    }

    char tmp1[kScreenHeight][kScreenWidth] = {
    "                                                   ",
    "################################################   ",
    "#                                              #   ",
    "#                                              #   ",
    "#                                              #   ",
    "#                                              #   ",
    "#                                              #   ",
    "#                                              #   ",
    "#                                              ####",
    "#                                               | #",
    "#                                               |+#",
    "#                                               | #",
    "#                                              ####",
    "#                                              #   ",
    "#                                              #   ",
    "#                                              #   ",
    "#                                              #   ",
    "#                                              #   ",
    "#                                              #   ",
    "#                                              #   ",
    "################################################   "
    };

    char tmp2[kScreenHeight][kScreenWidth] = {
        "                                                   ",
        "################################################   ",
        "#                                              #   ",
        "#                                              #   ",
        "#                                ###############   ",
        "#                                              #   ",
        "################                               #   ",
        "#                                              #   ",
        "#                                              ####",
        "#                                               | #",
        "#                                               |+#",
        "#                                               | #",
        "#                                              ####",
        "#                                              #   ",
        "#                                  #############   ",
        "#                                              #   ",
        "##########################                     #   ",
        "#                                              #   ",
        "#                                              #   ",
        "#                                              #   ",
        "################################################   "
    };

        char tmp3[kScreenHeight][kScreenWidth] = {
        "                                                   ",
        "################################################   ",
        "#                                              #   ",
        "#             #                    #           #   ",
        "#             #                  ###############   ",
        "#        #    #                    #           #   ",
        "################                   #           #   ",
        "#        #                         #           #   ",
        "#        #                                     ####",
        "#        #                                      | #",
        "#                                               |+#",
        "#                     #              #          | #",
        "#                     #              #         ####",
        "#        |            #              #    #    #   ",
        "#        |            #            #############   ",
        "#                     #              #         #   ",
        "##########################                   ###   ",
        "#                     #                        #   ",
        "#                     #                        #   ",
        "#                                              #   ",
        "################################################   "
        };


        char example[kScreenHeight][kScreenWidth] = {
        "################################################   \n",
        "#       X <- (Monster bodies)                  #   \n",
        "#                                              #   \n",
        "#          W <- (Witcher)            X         #   \n",
        "#                                              #   \n",
        "#                                              #   \n",
        "#              X                               #   \n",
        "#                                              ####\n",
        "#                        M <- (Actual monter)   | #\n",
        "#                                               |+#\n",
        "#                                               | #\n",
        "#                           X                  ####\n",
        "#                                              #   \n",
        "#              X                               #   \n",
        "#                                              #   \n",
        "#                                              #   \n",
        "#                                              #   \n",
        "#                                      X       #   \n",
        "#                                              #   \n",
        "################################################   \n"
        };
}

#endif // MAPS_H