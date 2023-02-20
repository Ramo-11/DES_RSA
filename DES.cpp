#include <iostream>
#include <unordered_map>
#include <vector>
#include <sstream>
#include <algorithm>

class DES {
    public:
        std::string key;
        std::string ciphertext;
        int keyp[56] = { 57, 49, 41, 33, 25, 17, 9,  1,  58, 50, 42, 34,
                    26, 18, 10, 2,  59, 51, 43, 35, 27, 19, 11, 3,
                    60, 52, 44, 36, 63, 55, 47, 39, 31, 23, 15, 7,
                    62, 54, 46, 38, 30, 22, 14, 6,  61, 53, 45, 37,
                    29, 21, 13, 5,  28, 20, 12, 4 
                };
        int shiftTable[16] = { 1, 1, 2, 2, 2, 2, 2, 2,
                            1, 2, 2, 2, 2, 2, 2, 1 };

        int keyComp[48] = { 14, 17, 11, 24, 1,  5,  3,  28,
                         15, 6,  21, 10, 23, 19, 12, 4,
                         26, 8,  16, 7,  27, 20, 13, 2,
                         41, 52, 31, 37, 47, 55, 30, 40,
                         51, 45, 33, 48, 44, 49, 39, 56,
                         34, 53, 46, 42, 50, 36, 29, 32 };

        int initial_perm[64] = { 58, 50, 42, 34, 26, 18, 10, 2,  60, 52, 44,
                            36, 28, 20, 12, 4,  62, 54, 46, 38, 30, 22,
                            14, 6,  64, 56, 48, 40, 32, 24, 16, 8,  57,
                            49, 41, 33, 25, 17, 9,  1,  59, 51, 43, 35,
                            27, 19, 11, 3,  61, 53, 45, 37, 29, 21, 13,
                            5,  63, 55, 47, 39, 31, 23, 15, 7 };
                            
        int expansionDBox[48] = { 32, 1,  2,  3,  4,  5,  4,  5,  6,  7,  8,  9,
                    8,  9,  10, 11, 12, 13, 12, 13, 14, 15, 16, 17,
                    16, 17, 18, 19, 20, 21, 20, 21, 22, 23, 24, 25,
                    24, 25, 26, 27, 28, 29, 28, 29, 30, 31, 32, 1 };

        int sBox[8][4][16] = {
        { 14, 4,  13, 1, 2,  15, 11, 8,  3,  10, 6,  12, 5,
          9,  0,  7,  0, 15, 7,  4,  14, 2,  13, 1,  10, 6,
          12, 11, 9,  5, 3,  8,  4,  1,  14, 8,  13, 6,  2,
          11, 15, 12, 9, 7,  3,  10, 5,  0,  15, 12, 8,  2,
          4,  9,  1,  7, 5,  11, 3,  14, 10, 0,  6,  13 },
        { 15, 1,  8,  14, 6,  11, 3, 4,  9,  7,  2,  13, 12,
          0,  5,  10, 3,  13, 4,  7, 15, 2,  8,  14, 12, 0,
          1,  10, 6,  9,  11, 5,  0, 14, 7,  11, 10, 4,  13,
          1,  5,  8,  12, 6,  9,  3, 2,  15, 13, 8,  10, 1,
          3,  15, 4,  2,  11, 6,  7, 12, 0,  5,  14, 9 },
 
        { 10, 0,  9,  14, 6,  3,  15, 5,  1,  13, 12,
          7,  11, 4,  2,  8,  13, 7,  0,  9,  3,  4,
          6,  10, 2,  8,  5,  14, 12, 11, 15, 1,  13,
          6,  4,  9,  8,  15, 3,  0,  11, 1,  2,  12,
          5,  10, 14, 7,  1,  10, 13, 0,  6,  9,  8,
          7,  4,  15, 14, 3,  11, 5,  2,  12 },
        { 7,  13, 14, 3,  0,  6,  9,  10, 1,  2, 8,  5,  11,
          12, 4,  15, 13, 8,  11, 5,  6,  15, 0, 3,  4,  7,
          2,  12, 1,  10, 14, 9,  10, 6,  9,  0, 12, 11, 7,
          13, 15, 1,  3,  14, 5,  2,  8,  4,  3, 15, 0,  6,
          10, 1,  13, 8,  9,  4,  5,  11, 12, 7, 2,  14 },
        { 2,  12, 4, 1,  7,  10, 11, 6, 8,  5,  3,  15, 13,
          0,  14, 9, 14, 11, 2,  12, 4, 7,  13, 1,  5,  0,
          15, 10, 3, 9,  8,  6,  4,  2, 1,  11, 10, 13, 7,
          8,  15, 9, 12, 5,  6,  3,  0, 14, 11, 8,  12, 7,
          1,  14, 2, 13, 6,  15, 0,  9, 10, 4,  5,  3 },
        { 12, 1,  10, 15, 9,  2,  6,  8,  0,  13, 3, 4, 14,
          7,  5,  11, 10, 15, 4,  2,  7,  12, 9,  5, 6, 1,
          13, 14, 0,  11, 3,  8,  9,  14, 15, 5,  2, 8, 12,
          3,  7,  0,  4,  10, 1,  13, 11, 6,  4,  3, 2, 12,
          9,  5,  15, 10, 11, 14, 1,  7,  6,  0,  8, 13 },
        { 4,  11, 2,  14, 15, 0,  8, 13, 3,  12, 9,  7,  5,
          10, 6,  1,  13, 0,  11, 7, 4,  9,  1,  10, 14, 3,
          5,  12, 2,  15, 8,  6,  1, 4,  11, 13, 12, 3,  7,
          14, 10, 15, 6,  8,  0,  5, 9,  2,  6,  11, 13, 8,
          1,  4,  10, 7,  9,  5,  0, 15, 14, 2,  3,  12 },
        { 13, 2,  8, 4,  6,  15, 11, 1,  10, 9, 3, 14, 5,
          0,  12, 7, 1,  15, 13, 8,  10, 3,  7, 4, 12, 5,
          6,  11, 0, 14, 9,  2,  7,  11, 4,  1, 9, 12, 14,
          2,  0,  6, 10, 13, 15, 3,  5,  8,  2, 1, 14, 7,
          4,  10, 8, 13, 15, 12, 9,  0,  3,  5, 6, 11 }
    };

        int per[32] = { 16, 7, 20, 21, 29, 12, 28, 17, 1,  15, 23,
            26, 5, 18, 31, 10, 2,  8,  24, 14, 32, 27,
            3,  9, 19, 13, 30, 6,  22, 11, 4,  25 };

        int finalPerm[64] = { 40, 8,  48, 16, 56, 24, 64, 32, 39, 7,  47,
                        15, 55, 23, 63, 31, 38, 6,  46, 14, 54, 22,
                        62, 30, 37, 5,  45, 13, 53, 21, 61, 29, 36,
                        4,  44, 12, 52, 20, 60, 28, 35, 3,  43, 11,
                        51, 19, 59, 27, 34, 2,  42, 10, 50, 18, 58,
                        26, 33, 1,  41, 9,  49, 17, 57, 25 };

        std::unordered_map<char, std::string> hexToBinMap;
        std::unordered_map<std::string, std::string> binToHexMap;
        
        DES() {
            // Initialize
            key = "8B2A7FF25E98C35D";
            
            hexToBinMap['0'] = "0000";
            hexToBinMap['1'] = "0001";
            hexToBinMap['2'] = "0010";
            hexToBinMap['3'] = "0011";
            hexToBinMap['4'] = "0100";
            hexToBinMap['5'] = "0101";
            hexToBinMap['6'] = "0110";
            hexToBinMap['7'] = "0111";
            hexToBinMap['8'] = "1000";
            hexToBinMap['9'] = "1001";
            hexToBinMap['A'] = "1010";
            hexToBinMap['B'] = "1011";
            hexToBinMap['C'] = "1100";
            hexToBinMap['D'] = "1101";
            hexToBinMap['E'] = "1110";
            hexToBinMap['F'] = "1111";

            binToHexMap["0000"] = "0";
            binToHexMap["0001"] = "1";
            binToHexMap["0010"] = "2";
            binToHexMap["0011"] = "3";
            binToHexMap["0100"] = "4";
            binToHexMap["0101"] = "5";
            binToHexMap["0110"] = "6";
            binToHexMap["0111"] = "7";
            binToHexMap["1000"] = "8";
            binToHexMap["1001"] = "9";
            binToHexMap["1010"] = "A";
            binToHexMap["1011"] = "B";
            binToHexMap["1100"] = "C";
            binToHexMap["1101"] = "D";
            binToHexMap["1110"] = "E";
            binToHexMap["1111"] = "F";

            ciphertext = 
            "D8C6C52158391C0AFD150E6B8E519BAA"
            "C44C60266DD2CB1B87344EFA0C06DB8C"
            "A82D850D49850723D7438B07F3C1F512"
            "512188DF2ECB5AB3D83FE100C74E2BA7"
            "6848D533A4538382EDEB4060DE88D43C"
            "7521B944DF91C69B03B8207C23A815A6"
            "1EF742C2D69B74CE184A03BE6500C54D"
            "E134A6EF66D839359D61CB06DFF5A995"
            "2EC3D9F6E6DCEC7B84C50069D91E368A"
            "596ABBD65B9EC0AB5F5979831D1FDBE3"
            "B0358D6B01C23290A1A4B6F640C454ED"
            "4EAAEC9BDEBE644DB9C95AD37A88F13B"
            "F29C3741F050E29BF699409BD0EC285C"
            "970D1416FB4D1B5F0685552090B7D5B6"
            "2E91DA5A83896E99D4C40DB066692380"
            "05B8183C9488376563EF011435884B2E"
            "1283E67A63ED9EF00AE70EEA86BEBC88"
            "7EE04D758A37769E3E9386D7140450BF"
            "9C696EF5F7B06E76FD404B0A1DA12449"
            "4559ED3DFD61708CAB9B39737B3DB0EB"
            "77C243828814B45937CBCECDD71A74F3"
            "040CF7F2073C2761CAECCA7EDAA8FFD9"
            "DC8F552E43262749F9C0E3C89852B518"
            "38FD7A68F377EDBF7628E2F48857C3D5"
            "3D6CB4B2BF119B8C3C3E1806B9F94414"
            "7CEFB4D2DA61F7990A34F7839D737907"
            "D3DE55435D263AD88918D2F522F89A5F"
            "EF9329D9D3E66388E7A973750FFD8DBD"
            "90776C65CD96109C256D34BE2CC60C35"
            "E284D300832F7862571790ECBB9AB7F0"
            "5CC87AE66DD4A66F7A2EC6DDD43A573A"
            "2DEA748A179C16BB45DD7C7978BE4DFC"
            "CB2B83F70A630F787FAA8E1019372C8C"
            "535A84DFF67923828A6B5447AA630F7D"
            "FF5328DDC66C044C0B88273A4ECA5043"
            "BE89ACA2CC60D429BF08FE5B05780B49"
            "2770F42FBB9DF36E6AB2E678AD43BD5D"
            "0A2A28D56756C85A380433BC1FFB7C2D"
            "8EF3C2F9AF592D058660DBEA0386E15B"
            "65558AB5CE49B424B99D7A6BE6B2788F"
            "1DD935E852D84891F2E54F26F4CDFC31"
            "A0A6785A2C94D4D8BEEBC8D23FFBA701"
            "E319D629BA83E55036057998A2A4495A"
            "FF8A99B6D3F5965DE1477B6B59685824"
            "0CF466A85E7D27CF885D4F26F2E721EB"
            "CE32A11C08B706F67ADE442CA79C8ADB"
            "D6DF16C40F10AF68B52A2A773054FFE4"
            "80B2BAD30908861EC4C490C8DAEEBDAD"
            "CE9C1CB3D20D4F0CF3A133FBCAEDC4C9"
            "EF88CA27558E01D97EF0BD3045605558"
            "27B7C3DB498E4B3A80A87D688B8F8751"
            "EF01981FC41ECD93802C1168D0D335A1"
            "54428A0EB045CE4BD2A8178FA7F223BE"
            "41CC97DDEEA416EFF435C65116EE5124"
            "38838AC7CD8FAF64EC37CECAB9EC7295"
            "B60B4FB2DDD3B571BA0EEDA9A18A455B"
            "E295F854B83A1950AA306BAD850192F2"
            "2D69D5D2F388F4091655D9291CF1B062"
            "4BCEE55AB6AA75EF82AD251908412655"
            "F53030F37A90BFB4640F4445FB74A158"
            "C0E55A63A10E3CE069FA226FE94A7F37"
            "D5F98795EABD9DE13DABC11DBDC04A71"
            "F67260B66E98F2F5E6F134C8B34C3D44"
            "FFC41D16944BFDF7FE9A4663E7F5041C"
            "7DD815447FD54123B796F70C960632E1"
            "31AE253CD81DA34B9DE739494BD9562F"
            "1C53FDA78B904D7157E6404CACDC7CA4"
            "2C9AB5C4146E7A550C5B08944E8D9791"
            "62720FC78B7F3AD54E8CE3383C1A839B"
            "9E8CFFAD500380D6ADE2B1DD086DD7C5"
            "287C3575E3FC39E5784F7FA61C1AD784"
            "D03EE00C81FC273C5332870F10414393"
            "77E2C67EC3FB20F7CAAA8AC37E765F6E"
            "884DC88CA2B593E41919E884D490F951"
            "3E5A094FFC3F8EF50E4005E398722532"
            "AB67370656D5D411576D795D92FF5E44"
            "21D02B8A0C3DEA60849DCF243C17A5DF"
            "3FB8B00B6F8D644DBD04A9B24930A097"
            "5588252174E35AD10149C0396BDE91A1"
            "F5BFE68B1A0251CC68A638BF0DD09DBA"
            "DE3936318E5F02F3EF48CF45CFF952B8"
            "2506AF26A3F26A15C1BFB348772A1BD6"
            "BE36A3BB5729E351F2AC0DE4F8F2BFDE"
            "F2A15D03818A5675A08C6A87D72DE737"
            "3EEE46B8D3F8FAC716E8D7718FC3B3D7"
            "BF8988737065ED12F7D371071EC84D03"
            "A18B78043AD62ABD2BF6D299375A95AC"
            "55BB684AD59541B119A4276BDDFC5DC7"
            "C981D3C29393FFB34CB1D6A3A0208404"
            "0D329CC331644110750DAD7DF23909FD"
            "3F05F6CB58AF3486A4BCCFCEA134BC42"
            "824505950F769B440F2D68A70BBA6A69"
            "73D9C492D0C38302D814D65BB15B7AA0"
            "4D0F9D4E4932B0127C28F71E6A7B1F8A"
            "54BAD53FA0484B71C9ADEEB2B41750BB"
            "72C7FCB6D77E991A7BBC45498F014AD7"
            "4D8CF31C01987180FE00BA3419435BAE"
            "AA1942326266197B6752030B2B5D4425"
            "72D93B7204A6D2712094ACD79F648970"
            "45B3E14D6653C5A465C5DC4EE0CAC20E"
            "62C6E13544D97DF8E4BEF0E7C8551FC1"
            "3833A245484D3C022C5C1377A0F3DE14"
            "6A0E9060B1C38B0EA34AD5B4CB05B77B"
            "8C518BFFF6BCEB9EB178B41E771521D7"
            "340AB68CF8AF440DBEC023C425EA163D"
            "95275C6D6925E3CF";
        }

        int hexToDec(std::string hex) {
            int decimal_value;
            std::stringstream ss;
            ss << std::hex << hex;
            ss >> decimal_value;
            return decimal_value;
        }

        std::string hexToBin(std::string hex) {
            std::string bin = "";
            for (int i = 0; i < hex.size(); i++) {
                bin += hexToBinMap[hex[i]];
            }
            return bin;
        }

        std::string binToHex(std::string bin) {
            std::string hex = "";
            for (int i = 0; i < bin.length(); i += 4) {
                std::string ch = "";
                ch += bin[i];
                ch += bin[i + 1];
                ch += bin[i + 2];
                ch += bin[i + 3];
                hex += binToHexMap[ch];
            }
            return hex;
        }
    
        std::string permute(std::string var, int arr[], int length) {
            std::string permute = "";
            for (int i = 0; i < length; i++) {
                permute += var[arr[i] - 1];
            }
            return permute;
        }

        std::string shiftLeft(std::string var, int shiftsNum) {
            std::string string = "";
            for (int i = 0; i < shiftsNum; i++) {
                for (int j = 1; j < 28; j++) {
                    string += var[j];
                }
                string += var[0];
                var = string;
                string = ""; // reset string
            }
            return var;
        }

        std::string xorFunction(std::string a, std::string b) {
            // Takes in a and b and return the xor result
            std::string result;
            for (int i = 0; i < a.size(); i++) {
                if (a[i] == b[i]) result += "0";
                else  result += "1";
            }
            return result;
        }

        std::string encryptPlainText(std::string plainText, std::vector<std::string> roundKey, std::vector<std::string> rk) {
            plainText = hexToBin(plainText);

            // Step 1: Apply initial permutation to plaintext       
            plainText = permute(plainText, initial_perm, 64);
        
            // Step 2: Split into left and right
            std::string left = plainText.substr(0, 32);
            std::string right = plainText.substr(32, 32);

            for (int i = 0; i < 16; i++) {
                std::string right_expanded = permute(right, expansionDBox, 48);
        
                // XOR RoundKey[i] and right_expanded
                std::string x = xorFunction(roundKey[i], right_expanded);
        
                std::string operationVar;
                for (int i = 0; i < 8; i++) {
                    int row = 2 * int(x[i * 6] - '0')
                            + int(x[i * 6 + 5] - '0');
                    int col = 8 * int(x[i * 6 + 1] - '0')
                            + 4 * int(x[i * 6 + 2] - '0')
                            + 2 * int(x[i * 6 + 3] - '0')
                            + int(x[i * 6 + 4] - '0');
                    int resultOfSBox = sBox[i][row][col];
                    operationVar += char(resultOfSBox / 8 + '0');
                    resultOfSBox = resultOfSBox % 8;
                    operationVar += char(resultOfSBox / 4 + '0');
                    resultOfSBox = resultOfSBox % 4;
                    operationVar += char(resultOfSBox / 2 + '0');
                    resultOfSBox = resultOfSBox % 2;
                    operationVar += char(resultOfSBox + '0');
                }
                operationVar = permute(operationVar, per, 32);
        
                // XOR left and op
                x = xorFunction(operationVar, left);
        
                left = x;
        
                if (i != 15) swap(left, right);
            }
        
            // Step x: combine left and right section after the 16 rounds
            std::string combine = left + right;
        
            std::string cipher = binToHex(permute(combine, finalPerm, 64));
            return cipher;
        }
};