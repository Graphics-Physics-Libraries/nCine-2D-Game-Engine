#include "apptest_scene.h"
#include <ncine/Application.h>
#include <ncine/Texture.h>
#include <ncine/Sprite.h>
#include <ncine/IFile.h> // for dataPath()
#include "apptest_datapath.h"

namespace {

#ifdef __ANDROID__
const char *Texture1File = "texture1_ETC2.ktx";
const char *Texture2File = "texture2_ETC2.ktx";
const char *Texture3File = "texture3_ETC2.ktx";
const char *Texture4File = "texture4_ETC2.ktx";
#else
const char *Texture1File = "texture1.png";
const char *Texture2File = "texture2.png";
const char *Texture3File = "texture3.png";
const char *Texture4File = "texture4.png";
#endif

}

nc::IAppEventHandler *createAppEventHandler()
{
	return new MyEventHandler;
}

void MyEventHandler::onPreInit(nc::AppConfiguration &config)
{
	setDataPath(config);
}

void MyEventHandler::onInit()
{
	pause_ = false;
	angle_ = 0.0f;

	nc::SceneNode &rootNode = nc::theApplication().rootNode();

	textures_[0] = nctl::makeUnique<nc::Texture>((nc::IFile::dataPath() + "textures/" + Texture1File).data());
	textures_[1] = nctl::makeUnique<nc::Texture>((nc::IFile::dataPath() + "textures/" + Texture2File).data());
	textures_[2] = nctl::makeUnique<nc::Texture>((nc::IFile::dataPath() + "textures/" + Texture3File).data());
	textures_[3] = nctl::makeUnique<nc::Texture>((nc::IFile::dataPath() + "textures/" + Texture4File).data());

	const float width = nc::theApplication().width();
	for (unsigned int i = 0; i < NumSprites; i++)
	{
		sprites_[i] = nctl::makeUnique<nc::Sprite>(&rootNode, textures_[i % NumTextures].get(), width * 0.15f + width * 0.1f * i, 0.0f);
		sprites_[i]->setScale(0.5f);
	}
}

void MyEventHandler::onFrameStart()
{
	const float height = nc::theApplication().height();

	if (pause_ == false)
	{
		angle_ += 1.0f * nc::theApplication().interval();
		if (angle_ > 360.0f)
			angle_ -= 360.0f;
	}

	for (unsigned int i = 0; i < NumSprites; i++)
	{
		sprites_[i]->y = height * 0.3f + fabsf(sinf(angle_ + 5.0f * i)) * (height * (0.25f + 0.02f * i));
		sprites_[i]->setRotation(angle_ * 20);
	}
}

#ifdef __ANDROID__
void MyEventHandler::onTouchDown(const nc::TouchEvent &event)
{
	pause_ = true;
}

void MyEventHandler::onTouchUp(const nc::TouchEvent &event)
{
	pause_ = false;
}
#endif

void MyEventHandler::onMouseButtonPressed(const nc::MouseEvent &event)
{
	if (event.isLeftButton())
		pause_ = true;
}

void MyEventHandler::onMouseButtonReleased(const nc::MouseEvent &event)
{
	if (event.isLeftButton())
		pause_ = false;
}

void MyEventHandler::onKeyReleased(const nc::KeyboardEvent &event)
{
	if (event.sym == nc::KeySym::ESCAPE || event.sym == nc::KeySym::Q)
		nc::theApplication().quit();
	else if (event.sym == nc::KeySym::SPACE)
	{
		const bool isSuspended = nc::theApplication().isSuspended();
		nc::theApplication().setSuspended(!isSuspended);
	}
}
