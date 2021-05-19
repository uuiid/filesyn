#include <DoodleLib/Metadata/Shot.h>
#include <DoodleLib/Metadata/Episodes.h>
#include <DoodleLib/Exception/Exception.h>
#include <DoodleLib/Metadata/MetadataFactory.h>
#include <DoodleLib/Metadata/ContextMenu.h>

#include <boost/format.hpp>
namespace doodle {

Shot::Shot()
    : Metadata(),
      p_shot(-1),
      p_shot_ab() {
}

Shot::Shot(std::weak_ptr<Metadata> in_metadata,
           decltype(p_shot) in_shot,
           decltype(p_shot_ab) in_shot_ab)
    : Metadata(std::move(in_metadata)),
      p_shot(in_shot),
      p_shot_ab(std::move(in_shot_ab)) {
  if (p_shot < 0)
    throw DoodleError{"shot无法为负"};
}

const int64_t& Shot::getShot() const noexcept {
  return p_shot;
}

void Shot::setShot(const int64_t& in_shot) {
  if (in_shot < 0)
    throw DoodleError{"shot无法为负"};

  p_shot = in_shot;
  save();
}

const std::string& Shot::getShotAb() const noexcept {
  return p_shot_ab;
}

void Shot::setShotAb(const std::string& ShotAb) noexcept {
  p_shot_ab = ShotAb;
  save();
}
EpisodesPtr Shot::getEpisodesPtr() const {
  auto k_ptr = std::dynamic_pointer_cast<Episodes>(getParent());
  if(!k_ptr)
    throw nullptr_error("没有集数");
  return k_ptr;
}

void Shot::setEpisodesPtr(const EpisodesPtr& Episodes_) noexcept {
    setParent(Episodes_);
}
std::string Shot::str() const {
  boost::format str_shot{"sc%04i%s"};
  str_shot % p_shot % p_shot_ab;
  return str_shot.str();
}

void Shot::load(const MetadataFactoryPtr& in_factory) {
  in_factory->load(this);
  p_metadata_flctory_ptr_ = in_factory;
}

void Shot::save(const MetadataFactoryPtr& in_factory) {
  p_metadata_flctory_ptr_ = in_factory;
  save();
}

bool Shot::operator<(const Shot& rhs) const {
  return std::tie(p_shot, p_shot_ab) < std::tie(rhs.p_shot, rhs.p_shot_ab);
}
bool Shot::operator>(const Shot& rhs) const {
  return rhs < *this;
}
bool Shot::operator<=(const Shot& rhs) const {
  return !(rhs < *this);
}
bool Shot::operator>=(const Shot& rhs) const {
  return !(*this < rhs);
}

bool Shot::sort(const Metadata& in_rhs) const {
  if (typeid(in_rhs) == typeid(*this)) {
    return *this < (dynamic_cast<const Shot&>(in_rhs));
  } else {
    return str() < in_rhs.str();
  }
}
void Shot::modifyParent(const std::shared_ptr<Metadata>& in_old_parent) {
  if (p_metadata_flctory_ptr_)
    p_metadata_flctory_ptr_->modifyParent(this, in_old_parent.get());
}
void Shot::createMenu(ContextMenu* in_contextMenu) {
  in_contextMenu->createMenu(std::dynamic_pointer_cast<Shot>(shared_from_this()));
}
void Shot::deleteData(const MetadataFactoryPtr& in_factory) {
  in_factory->deleteData(this);
}
void Shot::save() const {
  if(p_metadata_flctory_ptr_)
    p_metadata_flctory_ptr_->save(this);
}

}  // namespace doodle
