﻿#include "Gem.h"
//#include "Utils.h"
#include "Task.h"
#include "CCCallLambda.h"
//#include "BoardControls.h"
//#include "Board.h"
//#include "PlayerControl.h"
#include "RoundedRect.hpp"
namespace PH
{
    static const float kGemInterval = 0.5f;
    const size_t Gem::kGemWidthPixel = 107;
    const size_t Gem::kGemHeightPixel = 107;

	#define		NPC_FONT_SCALE 1.3f
	#define FONT_BMP_24					"fonts/arial.ttf"

//    
//    TaskPtr Gem::attachDarts(int turn,
//                             GemHook onLastTurn, GemOnSweepHook onSweep)
//    {
//        if(this->isBuffBusy())
//            return TaskIgnore::make();
//        
//        this->onBuffTurn = DefaultOnTurn;
//        this->onBuffLastTurn = onLastTurn;
//        this->onBuffSweep = onSweep;
//        
//        this->dartsBuff->set(turn, true);
//        this->buffFree = false;
//        
//        return
//        TaskSequence::make()
//        << this->modifyRootTexture(getNameWithPatternAndColor("feibiao_%s.png", this->color))
//        << this->attachCountdown(this->root, turn, kGemInterval)
//        << TaskAnim::make(this->root, CCRepeatForever::create(CCRotateBy::create(2.0f, 360.f)), false);
//    }
//    
//    TaskPtr Gem::sendDarts(const cocos2d::Vec2oint &dst)
//    {
//    }
    

    Gem::~Gem()
    {
        safeRelease(root);
    }
    
    void Gem::vSerialize(rapidjson::Value& o, rapidjson::Document::AllocatorType& alloc) const
    {
        o.AddMember("type", "Gem", alloc);
        o.AddMember("color", (int)mColor, alloc);
        o.AddMember("turn", mTurn, alloc);
    }
    
    void Gem::vDeserialize(rapidjson::Value& o)
    {
        assert(std::string(o["type"].GetString()) == "Gem" && "Gem: type mismatch");
        int col = o["color"].GetInt();
        int turn = o["turn"].GetInt();
        this->init(static_cast<GemUtils::GemColor>(col), turn);
    }
    
    void Gem::attachCountdown()
    {
        if(this->root == NULL) return;
        
        //float time = kGemInterval;
		std::stringstream stream;
		stream << mTurn;
        this->mCountdown = LabelBMFont::create(stream.str(), "bmfont/Molot_32_Energy.fnt");
        this->mCountdown->setAnchorPoint(Vec2(0.5f, 0.5f));
        this->mCountdown->setPosition(Gem::kGemWidthPixel * 0.60f,
                                      Gem::kGemHeightPixel * 0.25f);
        
        this->root->addChild(this->mCountdown);
        this->mCountdown->setOpacity(255);
        this->mCountdown->setScale(Director::getInstance()->getContentScaleFactor()*0.8f);
    }


	//Npc设置名字 称谓
	void Gem::SetShowName(const string& name, const string& title)
	{
		if (!name.size())
			return;
		this->root->removeChildByTag(ACTORCHILD_NAME, true);
		this->root->removeChildByTag(ACTORCHILD_TITLE, true);
		string strTitle = "<";
		strTitle += title + ">";
		LabelBMFont *pName = LabelBMFont::create(name.c_str(), "bmfont/FGDC_28_darkblue.fnt");
		pName->setScale(NPC_FONT_SCALE);
		pName->setAnchorPoint(Vec2(0.0f, 0.0f));

		LabelBMFont *pTitle = NULL;
		if (title.size()) {
			pTitle = LabelBMFont::create(strTitle.c_str(), "bmfont/FGDC_28_darkblue.fnt");
			pTitle->setAnchorPoint(Vec2(0.0f, 0.0f));
			pTitle->setScale(NPC_FONT_SCALE);
			this->root->addChild(pTitle, 0, ACTORCHILD_TITLE);
		}

		//if (GetSprite() != NULL && GetSprite()->IsDataLoaded() == true)
		{
			pName->setPosition(Vec2(0, 20 + 10));
			if (pTitle)
				pTitle->setPosition(Vec2(0, 20 + 10));
		}
		this->root->addChild(pName, 0, ACTORCHILD_NAME);
	}

	void Gem::open(float duration) {

		if (m_pCardFront && m_pCardBack) {
			m_pCardFront->stopAllActions();
			m_pCardBack->stopAllActions();

			if (this->root == NULL) return;

			this->root->setAnchorPoint(Vec2(0.0f, 0.0f));

			this->root->addChild(m_pCardFront);
			this->root->addChild(m_pCardBack);
			// 把牌反转了
			m_pCardFront->setFlipX(true);
			//m_pCardFront->setPosition(ccp(visibleSize.width / 2 - 100, visibleSize.height / 2 + 100));
			//m_pCardBack->setPosition(ccp(visibleSize.width / 2 - 100, visibleSize.height / 2 + 100));
			

			// 动画序列（延时，隐藏，延时，隐藏）
			Sequence *pBackSeq = Sequence::create(DelayTime::create(0.5f), Hide::create(), DelayTime::create(0.5f), Hide::create(), NULL);
			ScaleTo* pScaleBack = ScaleTo::create(1.2f, -1, 1);
			Spawn* pSpawnBack = Spawn::create(pBackSeq, pScaleBack, NULL);
			m_pCardBack->runAction(pSpawnBack);

			// 动画序列（延时，显示，延时，显示）
			Sequence *pFrontSeq = Sequence::create(DelayTime::create(0.5f), Show::create(), DelayTime::create(0.5f), Show::create(), NULL);
			ScaleTo* pScaleFront = ScaleTo::create(1.2f, -1, 1);
			Spawn* pSpawnFront = Spawn::create(pFrontSeq, pScaleFront, NULL);
			m_pCardFront->runAction(pSpawnFront);

			/*
			// 正面z轴起始角度为90度（向左旋转90度），然后向右旋转90度
			OrbitCamera* orbitFront = OrbitCamera::create(duration*0.5, 1, 0, 90, -90, 0, 0);
			// 正面z轴起始角度为0度，然后向右旋转90度
			OrbitCamera* orbitBack = OrbitCamera::create(duration*0.5, 1, 0, 0, -90, 0, 0);

			m_pCardFront->setVisible(false);
			// 背面向右旋转90度->正面向左旋转90度
			m_pCardBack->runAction(Sequence::create(
				Show::create(), orbitBack, CCHide::create(),
				CCTargetedAction::create(front, CCSequence::create(CCShow::create(), orbitFront, NULL)), NULL));
				*/
		}
		else {
			CCLOG("[ERROR] PokerSprite front or back not init.");
		}

		if (0){
			/*
			BlendFunc cbl = { GL_DST_COLOR, GL_ONE };
			m_animal->setBlendFunc(cbl);
			m_animal->setColor(Color3B::RED);
			m_animal->stopAllActions();
			m_animal->runAction(Sequence::create(DelayTime::create(0.2f), CallFunc::create([this]() {
				BlendFunc cbl = { GL_DST_COLOR, GL_ONE };
				m_animal->setBlendFunc(cbl);
				m_animal->setColor(Color3B::WHITE);
			}), nullptr));
			*/
		}
	}

    TaskPtr Gem::updateCountdown()
    {
        if(this->mCountdown == NULL ) return TaskIgnore::make();
        
        auto pre = Spawn::create(ScaleTo::create(0.25f, Director::getInstance()->getContentScaleFactor()*2),
                                   FadeTo::create(0.25f, 155),
                                   NULL);
        
        auto rev = Spawn::create(ScaleTo::create(0.25f, Director::getInstance()->getContentScaleFactor()),
                                   FadeTo::create(0.25f, 255),
                                   NULL);
        

        auto action = CCCallLambda::create([=]()
        {
			std::stringstream stream;
			stream << mTurn;
            this->mCountdown->setString(stream.str());
        });
        return TaskAnim::make(this->mCountdown, Sequence::create(pre, rev,action, NULL), false);
    }
    
    bool Gem::init(GemUtils::GemColor c, int turn)
    {
        mColor  = c;
        mTurn   = turn;
		isOpen = false;

		if (false == isOpen)
			this->root = GemUtils::GetSprite("zhanwei_biankuang.png");
		else
			this->root = GemUtils::GetSprite(GemUtils::res(c));

		// 加载牌的正反两面
		m_pCardFront = GemUtils::GetSprite("zhanwei_biankuang.png");
		m_pCardBack = GemUtils::GetSprite(GemUtils::res(c));

        this->root->setAnchorPoint(Vec2(0.5f, 0.5f));
		m_pCardFront->retain();
		m_pCardBack->retain();
        this->root->retain();
		
		_back = RoundedRect::create();
		_back->setPosition(Vec2(-Gem::kGemWidthPixel / 2, -Gem::kGemHeightPixel / 2));
		_back->setSizeAndColor(Vec2(500.f, 500.f), Color4F::BLACK);
		SetShowName("aa", "");
		//this->root->addChild(_back, -1);
		this->root->addChild(_back);

        //this->attachCountdown();
        
        return true;
    }

    TaskPtr Gem::fadeOut(float time)
    {
        if(time <= 0.f) time = kGemInterval;
        FiniteTimeAction * act = NULL;//, * act2 = NULL;
        act = Spawn::create(ScaleTo::create(time, 0.90f),
                              FadeOut::create(time),
                              NULL);
        
        return TaskAnim::make(this->root, act);
    }
    
    void Gem::setOpacity(size_t a)
    {
        this->root->setOpacity(a);
    }
    
    TaskPtr Gem::fadeIn(float time)
    {
        if(time <= 0.f) time = kGemInterval;
        
        auto act = Spawn::create(EaseBounceOut::create(ScaleTo::create(time, 1.f)),
                                   FadeIn::create(time), NULL);
        
        return TaskSequence::make()
        << TaskLambda::make([=]()
        {
            this->root->setOpacity(0);
            this->root->setScale(0.9f);
            //if(this->mCountdown) this->mCountdown->setOpacity(0);
        })
        << TaskAnim::make(this->root, act);

    }
    
    TaskPtr Gem::modifyDisplayFrameWithPatternAndColor(const std::string &pattern,
                                                       GemUtils::GemColor col,
                                                       float time)
    {
        if(time <= 0.f) time = kGemInterval;
        
        auto seq = TaskSequence::make();
        seq << this->fadeOut(time);
        auto res = GemUtils::getNameWithPatternAndColor(pattern, col);
        
        this->mColor = col;
        seq << TaskLambda::make([=]()
                                {
                                    this->root->setDisplayFrame(GemUtils::GetSprite(res)->displayFrame());
                                });
        seq << this->fadeIn(time);
        return seq;
    }
    
    TaskPtr Gem::transformColor(GemUtils::GemColor toColor, float time)
    {
        return this->modifyDisplayFrameWithPatternAndColor("gem_%s.png", toColor);
    }
    
    TaskPtr Gem::moveTo(const Vec2 &dst, float time)
    {
        if(time <= 0.f) time = kGemInterval;
        
        FiniteTimeAction * act = NULL;
        act = EaseElasticOut::create(MoveTo::create(time, g2w_center(this->position)), 1.2f);
        return TaskAnim::make(this->root, act, false);
    }
    
	/*
    TaskPtr Gem::sweep(BoardControl* ctrl)
    {
        auto batch = TaskBatch::make();
        
        if(ctrl)
            batch << ctrl->removeGemFromGrid(this->position);

        return batch;
    }
    
    TaskPtr Gem::update(BoardControl* ctrl)
    {
        return TaskIgnore::make();
    }
    */
    // if two gems are neighboring and same color
    bool isSameColorNeighbor(const GemPtr l, const GemPtr r)
    {
        Vec2 dis = (l->position - r->position);
        return ((std::abs(dis.x) + std::abs(dis.y)) <= 1) && r->color() == l->color();
    }
    
    // if a gem is in or neighboring a set with the same color
    bool isInOrNeighborSameColorGemSet(const GemSet& set, const GemPtr gem)
    {
        for(GemSet::const_iterator i = set.begin(); i != set.end(); i++)
        {
            if(isSameColorNeighbor(*i, gem))
                return true;
        }
        return false;
    }
    
    // find same color, neighboring gem sets
    GemSet intersectGemSet(const GemSet& l, const GemSet& r)
    {
        GemSet cross;
        
        for(GemSet::const_iterator ri = r.begin(); ri != r.end(); ri++)
        {
            if(isInOrNeighborSameColorGemSet(l, *ri))
                cross.insert(*ri);
        }
        return cross;
    }
#if 0

    // --- hand ---
    bool HandGem::init(GemUtils::GemColor col, int t, float d)
    {
        this->mColor = col;
        this->mTurn = t;
        this->mDamage = d;
        
        auto scale = Sequence::create(ScaleTo::create(.5f, 1.025f),
                                        ScaleTo::create(.5f, 1.f), NULL);
        auto blink = Sequence::create(FadeTo::create(.5f, 255),
                                        FadeTo::create(.5f, 235), NULL);
        auto spawn = Spawn::create(scale, blink, NULL);
        auto repeated = CCRepeatForever::create(spawn);
        
        this->root = GemUtils::GetSprite(GemUtils::getNameWithPatternAndColor("zhang_%s.png", col));
        this->root->setAnchorPoint(Vec2(0.5f, 0.5f));
        this->root->runAction(repeated);
        this->root->retain();
        
        mBackground = GemUtils::GetSprite(GemUtils::getNameWithPatternAndColor("zhang_bg_%s.png", col));
        auto size = this->root->getContentSize();
        mBackground->setAnchorPoint(Vec2(0.5f, 0.5f));
        mBackground->setPosition(Vec2(skGemPixelHeight/2, skGemPixelWidth/2));
        mBackground->runAction(RepeatForever::create(RotateBy::create(2.0f, 360.f)));
        mBackground->setOpacity(0);
        mBackground->setScale(0.95);
        this->root->addChild(mBackground,-1);
        
        this->attachCountdown();

        return true;
    }
    
    TaskPtr HandGem::transformColor(GemUtils::GemColor toColor, float time)
    {
        return this->modifyDisplayFrameWithPatternAndColor("zhang_%s.png", toColor);
    }
    
    TaskPtr HandGem::update(PH::BoardControl *ctrl)
    {
        auto seq = TaskSequence::make();
        
        mTurn --;
        seq << this->updateCountdown();
        if(mTurn <= 0){
            ctrl->damageFromGems += mDamage;
            seq << this->explosion();
            ctrl->removeGemFromGridRaw(this->position);
        }
        return seq;
    }
    
    TaskPtr HandGem::explosion()
    {
        auto hack = this->shared_from_this();
        return TaskSequence::make()
        << TaskLambda::make([=]()
        {
            // [HACK] for convinence
            auto parent = this->root->getParent();
            if(!parent) return TaskIgnore::make();
            
            auto medium = GemUtils::GetSprite(GemUtils::getNameWithPatternAndColor("zhang_%s.png", this->mColor));
            auto act = Spawn::create(ScaleTo::create(0.5f, 2.0f), FadeOut::create(0.5f), NULL);
            medium->setAnchorPoint(Vec2(0.5f, 0.5f));
            medium->setPosition(this->root->getPosition());
            medium->runAction(act);
            
            parent->addChild(medium);
            
            auto self = hack;
            return TaskIgnore::make();
        });
    }
    
    bool BombGem::init(GemUtils::GemColor col, int turn, float damage)
    {
        mColor  = col;
        mTurn   = turn;
        mDamage = damage;
        
        this->root = GemUtils::GetSprite(GemUtils::getNameWithPatternAndColor("bomb_%s.png", col));
        this->root->setAnchorPoint(Vec2(0.5f, 0.5f));
        this->root->retain();
        
        CCSprite* medium = GemUtils::GetSprite("fuze_1.png");
        auto size = this->root->getContentSize();
        medium->setAnchorPoint(Vec2(0.5f, 0.5f));
        //Vec2oint pos = g2w_center(this->position);
        medium->setPosition(Vec2(Gem::kGemWidthPixel-15, Gem::kGemHeightPixel-30));
        medium->setScale(0.5);
        
        auto anim =
        AnimationCache::getInstance()->animationByName("skill_bomb_fuze");
        auto repeated = RepeatForever::create(Animate::create(anim));
        
        medium->runAction(repeated);
        medium->setOpacity(255);
        this->root->addChild(medium);
        
        this->attachCountdown();

        return true;
    }
    
    TaskPtr BombGem::update(PH::BoardControl *ctrl)
    {
        auto seq = TaskSequence::make();
        
        mTurn --;
        seq << this->updateCountdown();
        if(mTurn <= 0){
            ctrl->damageFromGems += mDamage;
            seq << this->explosion();
            ctrl->removeGemFromGridRaw(this->position);
        }
        return seq;
    }
    
    TaskPtr BombGem::transformColor(GemUtils::GemColor toColor, float time)
    {
        return this->modifyDisplayFrameWithPatternAndColor("bomb_%s.png", toColor);
    }
    
    TaskPtr BombGem::explosion()
    {
        Sprite* medium = GemUtils::GetSprite("fuze_1.png");
        auto size = this->root->getContentSize();
        medium->setAnchorPoint(Vec2(0.5f, 0.5f));
        //Vec2oint pos = g2w_center(this->position);
        medium->setPosition(Vec2(Gem::kGemWidthPixel/2, Gem::kGemHeightPixel/2));
        medium->setOpacity(0);
        this->root->addChild(medium);
        
        auto anim = AnimationCache::getInstance()->animationByName("skill_explosion");
        
        auto hack = this->shared_from_this();
        TaskSequencePtr seq = TaskSequence::make();
        seq << TaskLambda::make([=]()
                                {
                                    medium->setOpacity(255);
                                })
        << TaskBatch::make(TaskAnim::make(medium, Animate::create(anim)),
                           TaskAnim::make(this->root, FadeOut::create(0.3f)))
        << TaskLambda::make([=]()
                            {
                                auto self = hack;
                            });
        
        return seq;
    }

    bool LockedGem::init(GemUtils::GemColor col, int turn)
    {
        mColor  = col;
        mTurn   = turn;
        
        this->root = GetSprite(GemUtils::res(col));
        this->root->setAnchorPoint(Vec2(0.5f, 0.5f));
        this->root->retain();
        
        CCSprite * chain = GetSprite("chain_1.png");
        chain->setAnchorPoint(Vec2(0.5f, 0.5f));
        chain->setPosition(Vec2(Gem::kGemWidthPixel/2, Gem::kGemHeightPixel/2-10));
        this->root->addChild(chain);
        
        chain->setOpacity(255);
        chain->setScale(0.75f);
        FiniteTimeAction * act = NULL;//, * act2 = NULL;
        act = Spawn::create(CCEaseBounceOut::create(ScaleTo::create(kGemInterval, 1.f)),
                              CCFadeIn::create(kGemInterval),
                              NULL);
        
        TaskSequencePtr seq = TaskSequence::make();
        seq << TaskAnim::make(chain, act);
        
        this->attachCountdown();
        return true;
    }
    
    TaskPtr LockedGem::sweep(BoardControl* ctrl)
    {
        auto seq = TaskSequence::make();
        auto normalGem = Gem::make(this->color());
        seq << this->detachChain(normalGem->root);
        seq << ctrl->replaceGemRaw(this->position, normalGem);
        return seq;
    }
    
    TaskPtr LockedGem::update(PH::BoardControl *ctrl)
    {
        auto seq = TaskSequence::make();
        
        mTurn --;
        seq << this->updateCountdown();
        if(mTurn <= 0){
            auto normalGem = Gem::make(this->color());
            seq << this->detachChain(normalGem->root);
            seq << ctrl->replaceGem(this->position, normalGem);
        }
        return seq;
    }
    
    TaskPtr LockedGem::detachChain(CCNode* node)
    {
        CCSprite* medium = GetSprite("chain_1.png");
        medium->setAnchorPoint(Vec2(0.5f, 0.5f));
        medium->setPosition(Vec2(Gem::kGemWidthPixel/2, Gem::kGemHeightPixel/2-10));
        
        node->addChild(medium);
        
        auto chain_anim =
        CCAnimationCache::sharedAnimationCache()->animationByName("skill_chain");
        
        return TaskSequence::make()
        << TaskSound::make("sound/v/scroll_unfold.mp3")
        << TaskAnim::make(medium, CCAnimate::create(chain_anim))
        << TaskLambda::make([=]()
        {
            medium->removeFromParent();
        });
    }
    
    bool PlagueGem::init(GemUtils::GemColor col,
                         int turn, float damage,
                         int globalLimits, int localLimits)
    {
        mColor              = col;
        mMaxTurn = mTurn    = turn;
        mDamage             = damage;
        mGlobalLimits       = globalLimits;
        mLocalLimits        = localLimits;
        
        this->root = GetSprite(GemUtils::getNameWithPatternAndColor("wenyi_%s.png", col));
        this->root->setAnchorPoint(Vec2(0.5f, 0.5f));
        this->root->retain();
        
        CCSprite* medium = GetSprite("wenyiguang1.png");
        auto size = this->root->getContentSize();
        medium->setAnchorPoint(Vec2(0.5f, 0.5f));
        //Vec2oint pos = g2w_center(this->position);
        medium->setPosition(Vec2(-10+Gem::kGemWidthPixel/2,
                                -10+Gem::kGemHeightPixel/2));
        
        auto anim =
            CCAnimationCache::sharedAnimationCache()->animationByName("skill_plague");
        auto repeated = CCRepeatForever::create(CCAnimate::create(anim));
        medium->runAction(repeated);
        
        medium->setOpacity(255);
        this->root->addChild(medium);
        
        this->attachCountdown();
        mReady = false;
        return true;
    }
    
    TaskPtr PlagueGem::fadeIn(float time)
    {
        if(time <= 0.f) time = kGemInterval;
        
        auto act = Spawn::create(CCEaseBounceOut::create(ScaleTo::create(time, 1.f)),
                                   CCFadeIn::create(time), NULL);
        
        return TaskSequence::make()
        << TaskLambda::make([=]()
        {
            this->root->setOpacity(0);
            this->root->setScale(0.9f);
            this->mReady = true;
        })
        << TaskAnim::make(this->root, act);
        
    }
    
    TaskPtr PlagueGem::update(PH::BoardControl *ctrl)
    {
        if(!mReady) return TaskIgnore::make();
        
        auto seq = TaskSequence::make();
        
        mTurn --;
        ctrl->damageFromGems += mDamage;
        if(mTurn <= 0)
        {
            auto normalGem = Gem::make(this->color());
            seq << ctrl->replaceGemRaw(this->position, normalGem);
        }
        else
        {
            seq << this->updateCountdown();
            
            auto grid = ctrl->getGrid();
            int plagueCount = 0;
            for (int i=0; i<grid.size(); i++)
            {
                //printf("%s->%s\n", typeid(*grid(i).get()).name(), typeid(PlagueGem).name());
                if( typeid(*grid(i).get()) == typeid(PlagueGem) )
                    plagueCount ++;
            }
            
            int pos = this->position[1]*grid.Width+this->position[0];
            
            std::vector<int> val;
            val.push_back(pos+1);
            val.push_back(pos-1);
            val.push_back(pos+grid.Width);
            val.push_back(pos-grid.Width);
            
            std::random_shuffle(val.begin(), val.end());
            int iter = -1;
        
            //printf("[START] -> (%d, %d)\n", pos/grid.Width, pos%grid.Width);
            auto infectBatch = TaskBatch::make();
            while( plagueCount < mGlobalLimits && ++iter < mLocalLimits)
            {
                int r1 = pos/grid.Width,        c1 = pos%grid.Width;
                int r2 = val[iter]/grid.Width,  c2 = val[iter]%grid.Width;
                
                //printf("[TRY] -> (%d, %d)\n", c2, r2);
                
                if(r1 != r2 && c1!= c2) continue;
                if( val[iter]<0 || val[iter]>=grid.size() ) continue;
                
                
                auto gem = grid(val[iter]);
                if(!gem->isModifiable()) continue;
                
                //printf("[PLAGUE] -> (%d, %d)\n", c2, r2);
                
                auto plague = PlagueGem::make(gem->color(),
                                              mMaxTurn,
                                              mDamage,
                                              mGlobalLimits, mLocalLimits);
                infectBatch << ctrl->replaceGemRaw(Vec2i(c2, r2), plague);
                plagueCount ++;
                iter ++;
            }
            
            seq << infectBatch;
        }
        return seq;
    }
    
    TaskPtr PlagueGem::transformColor(GemUtils::GemColor toColor, float time)
    {
        return this->modifyDisplayFrameWithPatternAndColor("wenyi_%s.png", toColor);
    }
    
    bool ShieldGem::init(GemUtils::GemColor color, int turn)
    {
        mColor  = color;
        mTurn   = turn;
        
        this->root = GetSprite(GemUtils::getNameWithPatternAndColor("shield_%s_1.png", color));
        this->root->setAnchorPoint(Vec2(0.5f, 0.5f));
        this->root->retain();
        
        this->attachCountdown();
        return true;
    }
    
    TaskPtr ShieldGem::detachShield(cocos2d::CCNode *node)
    {
        CCSprite* medium = GetSprite("broken_shield_1.png");
        medium->setAnchorPoint(Vec2(0.5f, 0.5f));
        medium->setPosition(Vec2(Gem::kGemWidthPixel/2-10, Gem::kGemHeightPixel/2));

        this->root->removeAllChildren();
        this->root->addChild(medium);

        auto chain_anim =
        CCAnimationCache::sharedAnimationCache()->animationByName("skill_broken_shield");

        medium->setOpacity(0);

        return TaskSequence::make()
        //<< this->fadeOut(0.3f)
        << TaskLambda::make([=](){ this->root->setOpacity(0); medium->setOpacity(255);})
        << TaskSound::make("sound/v/scroll_unfold.mp3")
        << TaskAnim::make(medium, CCAnimate::create(chain_anim));
    }
    
    TaskPtr ShieldGem::update(PH::BoardControl *ctrl)
    {
        auto seq = TaskSequence::make();
        
        mTurn --;
        seq << this->updateCountdown();
        if(mTurn <= 0){
            auto normalGem = Gem::make(this->color());
            seq << this->detachShield(normalGem->root);
            seq << ctrl->replaceGemRaw(this->position, normalGem);
        }
        return seq;
    }
    
    TaskPtr ShieldGem::transformColor(GemUtils::GemColor toColor, float time)
    {
        return this->modifyDisplayFrameWithPatternAndColor("shield_%s_1.png", toColor);
    }
#endif
}
