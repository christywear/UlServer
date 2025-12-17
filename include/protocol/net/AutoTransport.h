#pragma once

#include <memory>
#include <protocol/net/ITransport.h>
#include <protocol/net/NetTypes.h>

class EnetTransport;
class SteamTransport;

class AutoTransport : public ITransport {
public:
    enum class Mode { ENET, STEAM };

    explicit AutoTransport(Mode mode);

    bool send(PlayerId to, const Buffer& data) override;
    void poll() override;

private:
    Mode mode_;
    std::unique_ptr<EnetTransport>  enet_;
    std::unique_ptr<SteamTransport> steam_;
};