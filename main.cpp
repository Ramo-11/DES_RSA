#include "DES.cpp"
#include <fstream>
#include <sstream>

void printPlainTextInHex(DES des, std::vector<std::string> roundKey, std::vector<std::string> rk);
void convertHexToASCII();

int main() {
    DES des;

    std::string binaryKey = des.hexToBin(des.key);
    binaryKey = des.permute(binaryKey, des.keyp, 56);

    std::string left = binaryKey.substr(0, 28);
    std::string right = binaryKey.substr(28, 28);

    std::vector<std::string> roundKey;
    std::vector<std::string> rk; 
    for (int i = 0; i < 16; i++) {
        left = des.shiftLeft(left, des.shiftTable[i]);
        right = des.shiftLeft(right, des.shiftTable[i]);
 
        std::string combine = left + right;
 
        std::string RoundKey = des.permute(combine, des.keyComp, 48);
 
        roundKey.push_back(RoundKey);
        rk.push_back(des.binToHex(RoundKey));
    }

    printPlainTextInHex(des, roundKey, rk);
    convertHexToASCII();
    return 0;
}

void printPlainTextInHex(DES des, std::vector<std::string> roundKey, std::vector<std::string> rk) {
    std::ofstream outFile("Plain_Text_Hex");
    std::string cipher("");
	reverse(roundKey.begin(), roundKey.end());
	reverse(rk.begin(), rk.end());
    int var = 0;

	for(int i = 0; i < 3482; i++) {
		if(i != 0 && i % 16 == 0) {
            if (var == 1) {
                outFile << std::endl;
                var = 0;
            }
			std::string text = des.encryptPlainText(cipher, roundKey, rk);
            for (int i = 0; i < text.size(); i += 2) {
                // print the current two characters
                if(i == text.size() - 2)
                    outFile << text[i] << text[i+1];
                else
                    outFile << text[i] << text[i+1] << " ";
            }
            var++;
			cipher = "";
		}
		cipher += des.ciphertext[i];	
    }
    outFile.close();
}

void convertHexToASCII() {
    std::ifstream inFile("Plain_Text_Hex");
    std::ofstream outFile("Plain_Text");
    std::string line;
    std::string plainText;
    while(getline(inFile, line)) {
        std::stringstream ss(line);

        std::string asciiStr;
        while (ss.good()) {
            std::string hexVal;
            ss >> hexVal;

            int hexInt;
            sscanf(hexVal.c_str(), "%x", &hexInt);

            // convert int to ASCII character
            char asciiChar = (char) hexInt;

            asciiStr += asciiChar;
        }
        plainText += asciiStr;
    }
    outFile << plainText;

    outFile.close();
    inFile.close();
}