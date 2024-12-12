#pragma once


#include "LoadBalancing-cpp/inc/Client.h"
#include <unordered_map>
#include <regex>


using namespace ExitGames;
using namespace ExitGames::LoadBalancing;
using namespace std;

class MyPhotonListener : public ExitGames::LoadBalancing::Listener {
public:
    std::unordered_map<std::string, int> receivedEventData;
    std::unordered_map<std::string, std::string> receivedEventDataStrings;
    int currentTurn = 1;
    bool waitingForResponse = true;

    virtual void connectReturn(int errorCode, const ExitGames::Common::JString& errorString, const ExitGames::Common::JString& region, const ExitGames::Common::JString& cluster) override {
        if (errorCode == 0) {
            std::cout << "Po³¹czenie udane. Region: " << region << ", klaster: " << cluster << std::endl;
        }
        else {
            std::cout << "B³¹d po³¹czenia: " << errorString << std::endl;
        }
    }
    
    virtual void debugReturn(int debugLevel, const ExitGames::Common::JString& string) override {
        std::cout << "Debug Level: " << debugLevel << " - " << std::endl;
    }

    virtual void connectionErrorReturn(int errorCode) override {
        std::cout << "B³¹d po³¹czenia: " << errorCode << std::endl;
    }

    virtual void clientErrorReturn(int errorCode) override {
        std::cout << "B³¹d klienta: " << errorCode << std::endl;
    }

    virtual void warningReturn(int warningCode) override {
        std::cout << "Ostrze¿enie: " << warningCode << std::endl;
    }

    virtual void serverErrorReturn(int errorCode) override {
        std::cout << "B³¹d serwera: " << errorCode << std::endl;
    }

    virtual void leaveRoomReturn(int errorCode, const ExitGames::Common::JString& errorString) override {
        if (errorCode == 0) {
            std::cout << "Opuszczono pokój." << std::endl;
        }
        else {
            std::cout << "B³¹d podczas opuszczania pokoju: " << std::endl;
        }
    }

    virtual void joinLobbyReturn() override {
        std::cout << "Do³¹czono do lobby." << std::endl;
    }

    virtual void leaveLobbyReturn() override {
        std::cout << "Opuœci³eœ lobby." << std::endl;
    }

    virtual void onRoomListUpdate() override {
        std::cout << "Lista pokoi zosta³a zaktualizowana." << std::endl;
    }

    virtual void onRoomPropertiesChange(const ExitGames::Common::Hashtable&) override {
        std::cout << "W³aœciwoœci pokoju zosta³y zmienione." << std::endl;
    }

    virtual void onPlayerPropertiesChange(int playerNr, const ExitGames::Common::Hashtable&) override {
        std::cout << "W³aœciwoœci gracza " << playerNr << " zosta³y zmienione." << std::endl;
    }

    virtual void onAppStatsUpdate() override {
        std::cout << "Statystyki aplikacji zosta³y zaktualizowane." << std::endl;
    }

    virtual void onAvailableRegions(const ExitGames::Common::JVector<ExitGames::Common::JString>& regions,
        const ExitGames::Common::JVector<ExitGames::Common::JString>& regionsWithError) override {
        std::cout << "Dostêpne regiony." << std::endl;
    }

    virtual void onDirectConnectionEstablished(int connectionId) override {
        std::cout << "Bezpoœrednie po³¹czenie nawi¹zane. ID po³¹czenia: " << connectionId << std::endl;
    }

    virtual void onDirectConnectionFailedToEstablish(int connectionId) override {
        std::cout << "Nie uda³o siê nawi¹zaæ bezpoœredniego po³¹czenia. ID po³¹czenia: " << connectionId << std::endl;
    }

    virtual void onCustomOperationResponse(const ExitGames::Photon::OperationResponse& response) override {
        std::cout << "OdpowiedŸ na operacjê niestandardow¹." << std::endl;
    }

    virtual void joinRoomEventAction(int playerNr, const ExitGames::Common::JVector<int>& playernrs, const ExitGames::LoadBalancing::Player& player) override {
        std::cout << "Gracz " << playerNr << " do³¹czy³ do pokoju." << std::endl;
    }

    virtual void leaveRoomEventAction(int playerNr, bool isInactive) override {
        std::cout << "Gracz " << playerNr << (isInactive ? " jest nieaktywny" : " opuœci³ pokój") << "." << std::endl;
    }

    virtual void customEventAction(int playerNr, nByte eventCode, const ExitGames::Common::Object& eventContent) override {
        std::cout << "Odebrano niestandardowe zdarzenie od gracza " << playerNr << ", kod zdarzenia: " << eventCode << std::endl;

        // Sprawdzanie kodu zdarzenia, np. 1 dla wiadomoœci tekstowej
        if (eventCode == 1) {
            std::cout << "Odebrano niestandardowe zdarzenie od gracza " << playerNr << ", kod zdarzenia: " << eventCode << std::endl;

            if (eventCode == 1) { // Sprawdzenie kodu zdarzenia
                // Odczytanie wiadomoœci z obiektu eventContent
                std::cout << eventContent.toString();
                Common::JString message;

                eventContent.toString(message);

                std::string input = message.UTF8Representation().cstr();

                // Regex do dopasowywania par klucz-wartoœæ (dla stringów i intów)
                std::regex regexInt("\"([^\"]+)\"\\s*=\\s*(\\d+)");
                std::regex regexString("\"([^\"]+)\"\\s*=\\s*\"([^\"]+)\"");

                // Dopasowania liczb ca³kowitych
                auto beginInt = std::sregex_iterator(input.begin(), input.end(), regexInt);
                auto endInt = std::sregex_iterator();

                for (auto it = beginInt; it != endInt; ++it) {
                    std::smatch match = *it;
                    std::string key = match[1].str();
                    int value = std::stoi(match[2].str());
                    receivedEventData[key] = value; // Przechowywanie int w mapie
                    std::cout << "Klucz (int): " << key << ", Wartoœæ: " << value << std::endl;
                }

                // Dopasowania tekstów
                auto beginString = std::sregex_iterator(input.begin(), input.end(), regexString);
                auto endString = std::sregex_iterator();

                for (auto it = beginString; it != endString; ++it) {
                    std::smatch match = *it;
                    std::string key = match[1].str();
                    std::string value = match[2].str();
                    receivedEventDataStrings[key] = value; // Przechowywanie string w osobnej mapie
                    std::cout << "Klucz (string): " << key << ", Wartoœæ: " << value << std::endl;
                }

                // Wyœwietlanie pe³nej wiadomoœci na konsoli
                std::cout << "Wiadomoœæ: " << message.UTF8Representation().cstr() << std::endl;

                waitingForResponse = false;
            }
        }
    }


    virtual void disconnectReturn(void) override {
        std::cout << "Roz³¹czono." << std::endl;
    }
};

