#include "mod/Waila.h"

class KeyboardManager;
class LayoutManager;
class ScreenController;
class UIControlFactory;
class UIMeasureStrategy;
struct ITTSEventManager;

class IClientInstance;
class ScreenContext;
class UIScene;

namespace alvinqid {

Waila& Waila::getInstance() {
    static Waila instance;
    return instance;
}

std::string Waila::getModNameFromNamespace(std::string mNamespace) {
	std::string itemNamespace = mNamespace;
	std::string modName;
	if (itemNamespace.empty() || itemNamespace == "minecraft")
	{
		modName = "Minecraft";
	}
	else {
		modName.reserve(itemNamespace.size());
		bool cap = true;
		for (char c : itemNamespace) {
			if (c == '_') {
				modName.push_back(' ');
				cap = true;
			}
			else if (cap) {
				modName.push_back(static_cast<char>(std::toupper(static_cast<unsigned char>(c))));
				cap = false;
			}
			else {
				modName.push_back(c);
			}
		}
	}
	return modName;
}

LL_AUTO_TYPE_INSTANCE_HOOK(
    GeneralSettingsScreenControllerCtor,
    HookPriority::Normal,
    GeneralSettingsScreenController,
    &GeneralSettingsScreenController::$ctor,
    void*,
    std::shared_ptr<MainMenuScreenModel> model,
    bool isGlobalSettings,
    bool reloadTexturePacksOnExit,
    std::string const& initialPackId
) {
    return origin(model, true, true, initialPackId);
}

LL_AUTO_TYPE_INSTANCE_HOOK(
    ScreenViewCtorHook,
    HookPriority::Normal,
    ScreenView,
    &ScreenView::$ctor,
    void*,
    KeyboardManager& keyboardManager,
    std::shared_ptr<ScreenController> controller,
    std::unique_ptr<VisualTree> visualTree,
    std::shared_ptr<UIControlFactory> controlFactory,
    UIMeasureStrategy& measureStrategy,
    std::unique_ptr<LayoutManager> layoutManager,
    GamepadCursorData gamepadCursorData,
    std::weak_ptr<ITTSEventManager> ttsEventManager
) {
    void* self = origin(keyboardManager,controller,visualTree,controlFactory,measureStrategy,layoutManager,gamepadCursorData,ttsEventManager);
    
    ScreenView* sv = reinterpret_cast<ScreenView*>(self);
    screenView = sv;
    
    return self;
}

LL_AUTO_TYPE_INSTANCE_HOOK(
    MinecraftUIRenderContextCtor,
    HookPriority::Normal,
    MinecraftUIRenderContext,
    &MinecraftUIRenderContext::$ctor,
    void*,
    IClientInstance& client, ScreenContext& screenContext, UIScene const& currentScene
) {
    void* self = origin(client, screenContext, currentScene);
    
    MinecraftUIRenderContext* mm = reinterpret_cast<MinecraftUIRenderContext*>(self);
    mcuirc = mm;
    
    return self;
}

LL_AUTO_TYPE_INSTANCE_HOOK(
    ScreenViewRenderHook,
    HookPriority::Normal,
    ScreenView,
    &ScreenView::render,
    void,
    UIRenderContext& ctx
) {
    origin(ctx);
    
    if(!screenView || screenView == nullptr) return;
    
    if (screenView->mVisualTree->mRootControlName->mName == "hud_screen") {
		const int padding = 4;
		const int topMargin = 8;
		const int maxWidth = 200;
		const int maxHeight = 100;
		const int itemIconWidth = 24;

		Vec2 screenSize = clientInstance->getGuiData()->mScreenSizeData->clientUIScreenSize;
		Font& font = *mcuirc->mDebugTextFontHandle.mDefaultFont.get();
		auto fontHandlePtr = Bedrock::NonOwnerPointer<const FontHandle>(&mcuirc->mDebugTextFontHandle);

		TextMeasureData tmd = TextMeasureData{
				1,  // fontSize
				0, //linepadding
				true, // renderShadow
				false, // showColorSymbol
				false, // hideHyphen
		};
		CaretMeasureData cmd = CaretMeasureData{
				0, false
		};

		ILevel* level = clientInstance->getLocalPlayer()->getLevel();
		HitResult& hr = level->getHitResult();
		if (hr.mType == HitResultType::TILE) {
			auto& block = clientInstance->getRegion()->getBlock(hr.mBlock);
			auto& i18n = getI18n();
			std::string blockId = block->mBlockType->mNameInfo.mRawName.getString();
			std::string langKey = block->mBlockType->buildDescriptionId(block);
			std::string translated = i18n.get(langKey, nullptr);
			std::string subtitleStr = std::string("§o");
			std::string mNamespace = getModNameFromNamespace(block.mBlockType->mNameInfo.mNamespaceName);
			subtitleStr.append(mNamespace);
			const char* title = translated.c_str();
			const char* subtitle = subtitleStr.c_str();

			UIMeasureStrategy& meassurer = mcuirc->mMeasureStrategy;
			MeasureResult result = meassurer.measureText(fontHandlePtr, title, maxWidth, maxHeight, tmd, cmd);
			MeasureResult result2 = meassurer.measureText(fontHandlePtr, subtitle, maxWidth, maxHeight, tmd, cmd);

			float height = result.mSize.y + result2.mSize.y + tmd.linePadding;
			float width = std::max(result.mSize.x, result2.mSize.x) + itemIconWidth;
			float x = screenSize.x / 2 - width / 2 - padding;
			float y = topMargin;


			// Draw the background
			Amethyst::NinesliceHelper backgroundSlice(16, 16, 3, 3);
			mce::TexturePtr texture = ctx.getTexture("textures/ui/hud_text_bg", true);
			RectangleArea bgRect = RectangleArea(
				x,
				x + width + padding * 2,
				y,
				y + height + padding * 2);
			backgroundSlice.Draw(bgRect, &texture, &ctx);
			ctx.flushImages(mce::Color::WHITE, 1.0f, "ui_flush");

			// Create the itemstack
			ItemStack* stack = new ItemStack();
			const auto& cblock = block;
			stack->reinit(*cblock, 1);
			stack->mShowPickup = false;

            alignas(BaseActorRenderContext) unsigned char mem[sizeof(BaseActorRenderContext)];
            auto* barc = reinterpret_cast<BaseActorRenderContext*>(mem);
            barc->$ctor(mcuirc->mScreenContext, clientInstance, clientInstance.getMinecraftGame_DEPRECATED());
            auto& ir = barc->mItemRenderer;
			ir.renderGuiItemNew(&barc, stack, 0, (x + padding), y + padding, false, 1.0f, 1.0f, 1.25f, 0);
			ctx.flushImages(mce::Color::WHITE, 1.0f, "ui_flush");

			// Draw the text
			ctx.drawText(font,
				RectangleArea(
					x + padding + itemIconWidth,
					x + width + padding,
					y + padding, 
					height), title, mce::Color::WHITE, 1.0f, ui::TextAlignment::Center,
				tmd,
				cmd);
			ctx.drawText(font,
				RectangleArea(
					x + padding + itemIconWidth,
					x + width + padding,
					y + padding + result.mSize.y + tmd.linePadding, 
					height), subtitle, mce::Color::BLUE, 1.0f, ui::TextAlignment::Center,
				tmd,
				cmd);
			ctx.flushText(1.0f);
		}
	}
}

LL_AUTO_TYPE_INSTANCE_HOOK(
    ClientInstanceCtor,
    HookPriority::Normal,
    ClientInstance,
    &ClientInstance::$ctor,
    void*,
    ClientInstanceArguments&& args
) {
    void* self = origin(args);
    
    ClientInstance* ci = reinterpret_cast<ClientInstance*>(self);
    clientInstance = ci;
    
    return self;
}

bool Waila::load() {
    getSelf().getLogger().debug("Loading...");
    return true;
}

bool Waila::enable() {
    getSelf().getLogger().debug("Enabling...");
    return true;
}

bool Waila::disable() {
    getSelf().getLogger().debug("Disabling...");
    return true;
}

} // namespace alvinqid

LL_REGISTER_MOD(alvinqid::Waila, alvinqid::Waila::getInstance());