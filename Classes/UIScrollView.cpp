#include "UIScrollView.h"
//#include "CCEGLView.h"
//#include "UIBatchRenderer.h"
#ifdef _MSC_VER
	#pragma warning( disable: 4244 )
#endif
UIScrollView::UIScrollView(void)
{
	m_selectTag = -1;
	m_startPos = ccp(0,0);
	m_scrollType = ScrollType_X;
	maxPt.x = 0;
	maxPt.y = 0;
	m_endSpace = 10;
	m_space = 3;
	m_pSelectUI = NULL;
}

UIScrollView::~UIScrollView(void)
{
}


void updatePos( float& des, float& src , const float& t )
{
	float d = des - src;
	float absD = fabs(d);

	float moveDis = t * 600;

	if ( absD > 0.1 )
	{
		if ( absD < moveDis ) 
		{
			moveDis= absD;
		}

		if ( d > 0 )
			src += moveDis;
		else 
			src -= moveDis;	
	}
}

void UIScrollView::update(float t)
{
	UI::update(t);

	if ( !m_hasDrag )
	{
		if ( fabs(m_v.x) > 0.1 || fabs(m_v.y) > 0.1 )
		{
			if ( true ) //abs(m_v.x) <= 1 && abs(m_v.y) <= 1 )
			{
				GetEndPos()	;

				m_v.x = 0;
				m_v.y = 0;
			}
			else
			{
				m_startPos.x += m_v.x * t * 1000;	
				m_startPos.y += m_v.y * t * 1000;	

				float V = 10 * t * 1000;

				if ( m_v.x > 0 )
				{
					m_v.x -= V;

					if ( m_v.x < 0 )
						m_v.x = 0;
				}
				else
				{
					m_v.x += V;

					if ( m_v.x > 0 )
						m_v.x = 0;

				}
				if ( m_v.y > 0 )
				{
					m_v.y -= V;

					if ( m_v.y < 0 )
						m_v.y = 0;
				}
				else
				{
					m_v.y += V;

					if ( m_v.y > 0 )
						m_v.y = 0;

				}
			}
			
		}
		else
		{
			updatePos( m_desPos.x , m_startPos.x, t );	
			updatePos( m_desPos.y , m_startPos.y, t );
		}				
	}
}


void UIScrollView::OnEvent( TEventType msg, CCTouch* touch )
{
	switch( msg )
	{
	case EVENT_DOWN:
		{
			m_hasDrag = false;
			UIManager::Instance()->setOnlyMessage(this);
			UI * p = UI::getTouchUI(m_startPos.x,m_startPos.y,touch);
			if(p) 
			{
				p->OnEvent(msg, touch);
				p->m_down = true;
				m_pSelectUI = p;
			}
		}
		break;
	case EVENT_UP:
		{
			if(m_pSelectUI)
				m_pSelectUI->m_down = false;
			UIManager::Instance()->setOnlyMessage(NULL);
			if (m_hasDrag) 
			{
				m_hasDrag = false;
				GetScrollMax();				
				return;
			}
			UI * p = UI::getTouchUI(m_startPos.x,m_startPos.y,touch);
			if(p) 
			{
				p->OnEvent(msg, touch);
			}
		}
		break;

	case EVENT_DRAG:
		{			
			m_hasDrag = true;

			int x = touch->getLocationInView().x - touch->getPreviousLocationInView().x;
			int y = touch->getLocationInView().y - touch->getPreviousLocationInView().y;

			
			if ( (m_scrollType & ScrollType_X) != 0 )
			{
				if(m_startPos.x > 0 || m_startPos.x < maxPt.x )
				{
					m_startPos.x += x*0.3;
					m_v.x = x*0.3;
				}
				else
				{
					m_startPos.x += x;
					m_v.x = x;
				}
			}
			else
			{
				m_startPos.x = 0;
				m_v.x = 0;
			}

			if ( (m_scrollType & ScrollType_Y) != 0 )
			{
				if( m_startPos.y < 0 || m_startPos.y > maxPt.y )
				{
					m_startPos.y += -y*0.3;
					m_v.y = -y*0.3;
				}
				else
				{
					m_startPos.y += -y;
					m_v.y = -y;
				}

			}
			else
			{
				m_startPos.y = 0;
				m_v.y = 0;
			}
			
		}
		break;	
	}
	UI::OnEvent(msg,touch);
}

void UIScrollView::GetEndPos()
{
	m_desPos = m_startPos;
	if ( (m_scrollType & ScrollType_PAGE) != 0 )
	{
		if ( (m_scrollType & ScrollType_X) != 0 )
		{
			int z =  (int)-m_desPos.x;
			if ( z < 0 ) z = 0;
			int page = (z + m_width/2)/ m_width;
			m_desPos.x = -page * m_width;
		}

		if ( (m_scrollType & ScrollType_Y) != 0 )
		{
			int z =  (int)m_desPos.y;
			int page = (z + m_height/2) / m_height;
			m_desPos.y = page * m_height;
		}
	}

	if ( m_desPos.x > 0 )
	{
		m_desPos.x = 0;
	}
	else if ( m_desPos.x < maxPt.x )
	{
		m_desPos.x = maxPt.x;
	}		
	else
	{
		//m_desPos.x = m_desPos.x;
	}

	if ( m_desPos.y < 0 )
	{
		m_desPos.y = 0;
	}
	else if ( m_desPos.y > maxPt.y )
	{
		m_desPos.y = maxPt.y;
	}	
	else
	{
		//m_desPos.y = m_desPos.y;
	}
}

void getRootUI(UI* child,float* scale )
{
	if (child == NULL)
		return ;

	if (child->getScale() >= 1.0f)
	{
		getRootUI((UI*)child->getParent(),scale);
	}
	else
	{
		*scale = child->getScale();
	}
}

void UIScrollView::visit(cocos2d::Renderer* renderer, const cocos2d::Mat4& transform, uint32_t flags)
{
	
	// quick return if not visible
	/*
	if (!m_bIsVisible)
	{
		return;
	}
	*/
#ifdef UI_BATCH_RENDERER
    UIBatchRenderer::instance()->flush();
#endif // UI_BATCH_RENDERER
	auto director = Director::getInstance();
	auto glview = director->getOpenGLView();
	auto screenSize = glview->getFrameSize();
	auto resolutionSize = glview->getDesignResolutionSize();

	float xScale = screenSize.height / resolutionSize.width;
	float yScale = screenSize.height / resolutionSize.height;
        
	kmGLPushMatrix();

	CCRect rc;
	{
		float scissorscale = 1.0f;
		//find the scaled node in the list
		getRootUI(this,&scissorscale);

		rc.origin = convertToWorldSpaceAR(ccp(0, 0));
        rc.origin.x *= xScale;
        rc.origin.y *= yScale;
		rc.size.width = m_width * xScale * scissorscale;
		rc.size.height = m_height * yScale * scissorscale;


		//glEnable(GL_SCISSOR_TEST);
		myGLEnableScissorTest();
		glScissor( rc.origin.x+m_space, rc.origin.y+m_space, (rc.size.width-m_space*2), (rc.size.height-m_space*2) );

		UIManager::Instance()->m_nonceClipRect.origin.x = rc.origin.x+m_space;
		UIManager::Instance()->m_nonceClipRect.origin.y = rc.origin.y+m_space;
		UIManager::Instance()->m_nonceClipRect.size.width = (rc.size.width-m_space*2);
		UIManager::Instance()->m_nonceClipRect.size.height = (rc.size.height-m_space*2);
	}

	//this->transform();

	CCNode* pNode = NULL;
	unsigned int i = 0;
	auto& children = getChildren();

	if (&children && children.size() > 0)
    {
        sortAllChildren();
		// draw children zOrder < 0

		for (int i = 0; i < children.size(); ++i)
        //ccArray *arrayData = m_pChildren->data;
        //for( ; i < arrayData->num; i++ )
        {
			pNode = (CCNode*)children.at(i);

            if ( pNode && pNode->getZOrder() < 0 ) 
            {
				UI* pp = dynamic_cast<UI*>(pNode);
				if(pp != NULL)
				{
					if(pp->getTag() == m_selectTag)
						pp->m_down = true;
					else
						pp->m_down = false;
				}
				pNode->visit();
				//UIBatchRenderer::instance()->flush();
				
            }
            else
            {
                break;
            }
        }
		
		glScissor( rc.origin.x, rc.origin.y, rc.size.width, rc.size.height );
	
		
		// self draw
		
		//this->draw();
#ifdef UI_BATCH_RENDERER
		UIBatchRenderer::instance()->flush();
#endif // UI_BATCH_RENDERER
		
		kmGLTranslatef(m_startPos.x, m_startPos.y, 0);
		UIManager::Instance()->m_TranslatefPos = m_startPos;

		glScissor( rc.origin.x+m_space, rc.origin.y+m_space, (rc.size.width-m_space*2), (rc.size.height-m_space*2) );

		/*
        for( ; i < arrayData->num; i++ )
        {
            pNode = (CCNode*) arrayData->arr[i];
            if (pNode)
            {
				UI* pp = dynamic_cast<UI*>(pNode);
				if(pp != NULL)
				{
					if(pp->getTag() == m_selectTag)
						pp->m_down = true;
					else
						pp->m_down = false;
				}
				
                pNode->visit();
#ifdef UI_BATCH_RENDERER
				UIBatchRenderer::instance()->flush();
#endif // UI_BATCH_RENDERER
				
            }
        }
		*/
    }
    else
    {
		
        //this->draw();
#ifdef UI_BATCH_RENDERER
		//UIBatchRenderer::instance()->flush();
#endif // UI_BATCH_RENDERER

    }

	 UIManager::Instance()->m_TranslatefPos = ccp(0,0);
	 //glDisable( GL_SCISSOR_TEST);
	 myGLDisableScissorTest();
	kmGLPopMatrix();
}


void UIScrollView::GetScrollMax()
{
	/*
	auto& children = getChildren();

	if ( children.size == 0 ) return;

	int maxX = 0;
	int minY = 0;


	ccArray *arrayData = m_pChildren->data;
	for( int i=0; i < (int)arrayData->num; i++ )
	{
		UI* pUI = dynamic_cast<UI*> ( arrayData->arr[i] );

		if ( pUI && pUI->isVisible() )
		{
			if ( pUI->m_width + pUI->getPosition().x > maxX )
			{
				maxX = pUI->m_width + pUI->getPosition().x;	
			}

			if ( pUI->getPosition().y < minY )
			{
				minY = pUI->getPosition().y;	
			}
		}		
	}		

	minY -= m_endSpace;
	maxX += m_endSpace;
	maxPt.x = -(maxX - m_width);
	maxPt.y = -minY;
	

	if ( (m_scrollType & ScrollType_PAGE) != 0 )
	{
		maxPt.x = (((int)maxPt.x - m_width+1) / (int)m_width) * m_width;
		maxPt.y = (((int)maxPt.y + m_width -1) / (int)m_height) * m_height;
	}

	if ( maxPt.x > 0 ) maxPt.x =0;
	if ( maxPt.y < 0 ) maxPt.y =0;
	*/
}

void UIScrollView::setInitPos(cocos2d::CCPoint point)
{
	GetScrollMax();
	CCPoint pt = point;
	if(point.x <= maxPt.x)
		pt.x = maxPt.x;

	if(point.x >= 0)
		pt.x = 0;

	if(point.y >= maxPt.y)
		pt.y = maxPt.y;

	if(point.y <= 0)
		pt.y = 0;

	m_startPos = pt; 
	m_desPos = pt;
}

void	UIScrollView::resetEndPos()
{
	GetScrollMax();
	if ( (m_scrollType & ScrollType_X) != 0 )
		m_startPos.x = maxPt.x;
	else
		m_startPos.x = 0;

	if ( (m_scrollType & ScrollType_Y) != 0 ) 
		m_startPos.y = maxPt.y;
	else
		m_startPos.y =0;

	m_desPos = m_startPos; 
	maxPt = m_startPos; 
}

void UIScrollView::removeAllChildrenWithCleanup(bool cleanup)
{
	m_pSelectUI = NULL;
	CCNode::removeAllChildrenWithCleanup( cleanup );
}
