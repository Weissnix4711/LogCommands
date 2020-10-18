#include <string>
#include <map>
#include <dllentry.h>
#include <base/log.h>
#include "base/playerdb.h"
#include <Actor/Actor.h>
#include <Packet/TextPacket.h>
#include <Actor/Player.h>

#include <boost/scope_exit.hpp>
#include <boost/format.hpp>
#include <boost/algorithm/string.hpp>

DEF_LOGGER("SampleMod");

// PS: Reserved for compatibility purposes
// If it is not necessary, keep both functions empty
// Initialization can be done in the PreInit function
// Incorrect use of this function may cause a crash
void dllenter() {}
void dllexit() {}

void PreInit() {}
void PostInit() {}

THook(
    void *, "?handle@ServerNetworkHandler@@UEAAXAEBVNetworkIdentifier@@AEBVCommandRequestPacket@@@Z",
    ServerNetworkHandler &snh, NetworkIdentifier const &netid, void *pk) {
  auto &db = Mod::PlayerDatabase::GetInstance();
  auto it  = db.Find(netid);

  if (!it || it->player->getCommandPermissionLevel() > CommandPermissionLevel::Any) { return original(snh, netid, pk); }

  std::string command(direct_access<std::string>(pk, 0x28));
  command = command.substr(1);
  std::vector<std::string> results;
  boost::split(results, command, [](char c) { return c == ' '; });

  LOGV("%s used: %s") % it->name % command;

  return original(snh, netid, pk);
}
