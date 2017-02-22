#include <hashlib++/hashlibpp.h>
#include <string>
#include <iostream>


int main()
{
    hashwrapper *myWrapper = new md5wrapper();
    std::string hash1 = myWrapper->getHashFromString("34010000002100000004:3401000000:0000");
    std::string hash2 = myWrapper->getHashFromString("REGISTER:sip:34010000002000010221@192.168.1.127:5060");
    std::string hash3 = myWrapper->getHashFromString(hash1 + ":" + "1467962310-de7f319482f8e0febd97eb139cad6d40:" + hash2);
    std::cout << hash3 << std::endl;
}
