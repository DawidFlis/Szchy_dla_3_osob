
#include "Plansza.h"

#include <atomic>

#include "MyPhotonListener.h"


using namespace ExitGames;
using namespace ExitGames::LoadBalancing;
using namespace std;


//std::atomic<bool> running(true); // Flaga kontroluj�ca prac� w�tku

//class MyPhotonListener : public ExitGames::LoadBalancing::Listener {
//public:
//    int currentTurn = 1;
//    bool waitingForResponse = true;
//
//    virtual void connectReturn(int errorCode, const ExitGames::Common::JString& errorString, const ExitGames::Common::JString& region, const ExitGames::Common::JString& cluster) override {
//        if (errorCode == 0) {
//            std::cout << "Po��czenie udane. Region: " << region << ", klaster: " << cluster << std::endl;
//        }
//        else {
//            std::cout << "B��d po��czenia: " << errorString << std::endl;
//        }
//    }
//    // Inne metody Listenera mo�na zaimplementowa� w miar� potrzeb
//    virtual void debugReturn(int debugLevel, const ExitGames::Common::JString& string) override {
//        std::cout << "Debug Level: " << debugLevel << " - " << std::endl;
//    }
//
//    virtual void connectionErrorReturn(int errorCode) override {
//        std::cout << "B��d po��czenia: " << errorCode << std::endl;
//    }
//
//    virtual void clientErrorReturn(int errorCode) override {
//        std::cout << "B��d klienta: " << errorCode << std::endl;
//    }
//
//    virtual void warningReturn(int warningCode) override {
//        std::cout << "Ostrze�enie: " << warningCode << std::endl;
//    }
//
//    virtual void serverErrorReturn(int errorCode) override {
//        std::cout << "B��d serwera: " << errorCode << std::endl;
//    }
//
//    virtual void leaveRoomReturn(int errorCode, const ExitGames::Common::JString& errorString) override {
//        if (errorCode == 0) {
//            std::cout << "Opuszczono pok�j." << std::endl;
//        }
//        else {
//            std::cout << "B��d podczas opuszczania pokoju: " << std::endl;
//        }
//    }
//
//    virtual void joinLobbyReturn() override {
//        std::cout << "Do��czono do lobby." << std::endl;
//    }
//
//    virtual void leaveLobbyReturn() override {
//        std::cout << "Opu�ci�e� lobby." << std::endl;
//    }
//
//    virtual void onRoomListUpdate() override {
//        std::cout << "Lista pokoi zosta�a zaktualizowana." << std::endl;
//    }
//
//    virtual void onRoomPropertiesChange(const ExitGames::Common::Hashtable&) override {
//        std::cout << "W�a�ciwo�ci pokoju zosta�y zmienione." << std::endl;
//    }
//
//    virtual void onPlayerPropertiesChange(int playerNr, const ExitGames::Common::Hashtable&) override {
//        std::cout << "W�a�ciwo�ci gracza " << playerNr << " zosta�y zmienione." << std::endl;
//    }
//
//    virtual void onAppStatsUpdate() override {
//        std::cout << "Statystyki aplikacji zosta�y zaktualizowane." << std::endl;
//    }
//
//    virtual void onAvailableRegions(const ExitGames::Common::JVector<ExitGames::Common::JString>& regions,
//        const ExitGames::Common::JVector<ExitGames::Common::JString>& regionsWithError) override {
//        std::cout << "Dost�pne regiony." << std::endl;
//    }
//
//    virtual void onDirectConnectionEstablished(int connectionId) override {
//        std::cout << "Bezpo�rednie po��czenie nawi�zane. ID po��czenia: " << connectionId << std::endl;
//    }
//
//    virtual void onDirectConnectionFailedToEstablish(int connectionId) override {
//        std::cout << "Nie uda�o si� nawi�za� bezpo�redniego po��czenia. ID po��czenia: " << connectionId << std::endl;
//    }
//
//    virtual void onCustomOperationResponse(const ExitGames::Photon::OperationResponse& response) override {
//        std::cout << "Odpowied� na operacj� niestandardow�." << std::endl;
//    }
//
//    virtual void joinRoomEventAction(int playerNr, const ExitGames::Common::JVector<int>& playernrs, const ExitGames::LoadBalancing::Player& player) override {
//        std::cout << "Gracz " << playerNr << " do��czy� do pokoju." << std::endl;
//    }
//
//    virtual void leaveRoomEventAction(int playerNr, bool isInactive) override {
//        std::cout << "Gracz " << playerNr << (isInactive ? " jest nieaktywny" : " opu�ci� pok�j") << "." << std::endl;
//    }
//
//    virtual void customEventAction(int playerNr, nByte eventCode, const ExitGames::Common::Object& eventContent) override {
//        std::cout << "Odebrano niestandardowe zdarzenie od gracza " << playerNr << ", kod zdarzenia: " << eventCode << std::endl;
//        
//        // Sprawdzanie kodu zdarzenia, np. 1 dla wiadomo�ci tekstowej
//        if (eventCode == 1) {
//            // Odczytanie wiadomo�ci z obiektu eventContent
//            cout << eventContent.toString();
//            Common::JString message;
//            
//            eventContent.toString(message);
//            
//            
//            string input = message.UTF8Representation().cstr();
//
//            std::regex regex("\"([^\"]+)\"\\s*=\\s*(\\d+)");
//
//                std::smatch matches;
//
//            // Wyszukiwanie wszystkich dopasowa�
//            auto begin = std::sregex_iterator(input.begin(), input.end(), regex);
//            auto end = std::sregex_iterator();
//
//            // Iterowanie po wszystkich dopasowaniach
//            for (auto it = begin; it != end; ++it) {
//                std::smatch match = *it;
//
//                // match[1] - klucz, match[2] - warto��
//                std::string key = match[1].str();
//                int value = std::stoi(match[2].str());
//                currentTurn = value;
//                std::cout << "Klucz: " << key << ", Wartosc: " << value << std::endl;
//            }
//
//
//            // Wy�wietlanie wiadomo�ci na konsoli
//            cout << "Wiadomo��: " << message.UTF8Representation().cstr() << endl;
//
//            waitingForResponse = false;
//        }
//    }
//
//
//    virtual void disconnectReturn(void) override {
//        std::cout << "Roz��czono." << std::endl;
//    }
//};




//void clientServiceLoop(Client& client) {
//    while (running) {
//        client.service();
//        std::this_thread::sleep_for(std::chrono::milliseconds(100));
//    }
//}


int main()
{

    //const ExitGames::Common::JString appId = "3bf3141a-2fda-4b2f-add5-cb80b8d0bb1f";          // Zast�p APP_ID swoim APP_ID z Photon Dashboard
    //const ExitGames::Common::JString appVersion = "1.0";             // Wersja aplikacji
    //const ExitGames::Common::JString region = "us";                  // Region serwera

    //// Inicjalizacja klienta i nas�uchiwanie zdarze�
    //MyPhotonListener listener;
    //ExitGames::LoadBalancing::Client client(listener, appId, appVersion);

    //// Po��czenie z serwerem
    //client.connect(/*ExitGames::LoadBalancing::ConnectOptions().setServerAddress(region)*/);

    //bool roomCreated = false;
    //bool attemptedJoinOrCreate = false;


    //std::thread serviceThread(clientServiceLoop, std::ref(client));
    //// G��wna p�tla aplikacji
    //while (running) {
    //    if (client.getState() == PeerStates::JoinedLobby && !attemptedJoinOrCreate) {
    //        Common::JString roomName = "MyRoom1";
    //        client.opJoinOrCreateRoom(roomName);
    //        attemptedJoinOrCreate = true;
    //    }

    //    if (client.getState() == PeerStates::Joined) {
    //        int playerNr = client.getLocalPlayer().getNumber();

    //        if (listener.currentTurn == playerNr) {
    //            int liczba;
    //            std::cout << "Twoja kolej! Podaj liczb�: ";
    //            std::cin >> liczba;

    //            Common::Hashtable eventContent;
    //            eventContent.put("liczba", liczba);

    //            int nextTurn = (playerNr % 3) + 1;
    //            eventContent.put("turn", nextTurn);

    //            nByte eventCode = 1;
    //            ExitGames::LoadBalancing::RaiseEventOptions options;
    //            options.setReceiverGroup(Lite::ReceiverGroup::ALL);
    //            bool eventSent = client.opRaiseEvent(true, eventContent, eventCode, options);

    //            std::cout << "Zdarzenie wys�ane: " << (eventSent ? "sukces" : "b��d") << std::endl;

    //            listener.waitingForResponse = true;
    //            while (listener.waitingForResponse) {
    //                std::this_thread::sleep_for(std::chrono::milliseconds(100)); // Ma�e op�nienie
    //            }
    //        }
    //    }
    //
    //}
    //running = false;
    //serviceThread.join();



    Plansza p;
    p.glowna_petla();
     
    return 0;
}

