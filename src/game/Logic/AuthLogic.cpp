#include <Game/Logic/AuthLogic.h>
#include <Game/Verbs.h>
#include <Game/LmLog.h>
#include <core/LyraDefs.h>
#include <DB/LmPlayerDBC.h>
#include <protocol/net/NetTypes.h>
#include <protocol/net/BinaryReader.h>
#include <game/CommandContext.h>
#include <cstdint>
#include <string>

namespace AuthLogic {

    // --- PHASE 1: PRE-LOGIN ---
    void HandlePreLogin(PlayerId from, BinaryReader& reader, CommandContext& ctx) {
        uint16_t version = 0;
        try {
            version = reader.read<uint16_t>();
        }
        catch (...) {
            LmLog::Instance()->Warning("PreLogin: Malformed packet from %llu", from);
            return;
        }

        LmLog::Instance()->Log("PreLogin: ID=%llu Ver=%d", from, version);

        uint16_t status = 'O';
        std::string challenge = "SALT_12345";

        if (version < Lyra::GAME_VERSION) {
            status = 'V';
            LmLog::Instance()->Warning("Rejected: Old Version (%d < %d)", version, Lyra::GAME_VERSION);
        }

        // FIX 1: No (uint16_t) cast! Pass the enum directly.
        ctx.Reply(VerbId::PRELOGIN_ACK, status, challenge);
    }

    // --- PHASE 2: LOGIN ---
    void HandleLogin(PlayerId from, BinaryReader& reader, CommandContext& ctx) {
        std::string username;
        std::string password;
        uint16_t version = 0;

        try {
            username = reader.readString();
            password = reader.readString();
            version = reader.read<uint16_t>();
        }
        catch (...) {
            LmLog::Instance()->Warning("Login: Packet parse error from %llu", from);
            return;
        }

        LmLog::Instance()->Log("Login: %s (Ver: %d)", username.c_str(), version);

        if (version < Lyra::GAME_VERSION) {
            // FIX 2: No (uint16_t) cast here either!
            ctx.Reply(VerbId::LOGIN_ACK, (int)'V', (lyra_id_t)0, "Client update required.");
            return;
        }

        lyra_id_t pid = LmPlayerDBC::Instance()->VerifyUser(username, password);
        uint16_t status = 'O';
        std::string msg = "Welcome to Underlight 2025!";

        if (pid == 0) {
            status = 'P';
            msg = "Invalid username or password.";
            LmLog::Instance()->Warning("Auth Failed for %s", username.c_str());
        }
        else {
            LmLog::Instance()->Log("Auth Success: %s -> ID %u", username.c_str(), pid);
        }

        // FIX 3: Removed cast. Enum goes in -> Template handles the int conversion internally.
        ctx.Reply(VerbId::LOGIN_ACK, status, pid, msg);
    }
}