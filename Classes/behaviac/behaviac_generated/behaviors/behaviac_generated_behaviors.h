﻿// ---------------------------------------------------------------------
// THIS FILE IS AUTO-GENERATED BY BEHAVIAC DESIGNER, SO PLEASE DON'T MODIFY IT BY YOURSELF!
// ---------------------------------------------------------------------

#ifndef _BEHAVIAC_GENERATED_BEHAVIORS_H_
#define _BEHAVIAC_GENERATED_BEHAVIORS_H_

#include "../types/behaviac_types.h"

namespace behaviac
{
	// Source file: test

	class Assignment_bt_test_node1 : public Assignment
	{
	public:
		BEHAVIAC_DECLARE_DYNAMIC_TYPE(Assignment_bt_test_node1, Assignment);
		Assignment_bt_test_node1()
		{
			m_bCast = true;
		}
	protected:
		virtual EBTStatus update_impl(Agent* pAgent, EBTStatus childStatus)
		{
			BEHAVIAC_UNUSED_VAR(pAgent);
			BEHAVIAC_UNUSED_VAR(childStatus);
			EBTStatus result = BT_SUCCESS;
			int opr = 5;
			((FirstAgent*)pAgent)->_Get_Property_<PROPERTY_TYPE_FirstAgent_p1, int >() = (int)opr;
			return result;
		}
	};

	class Condition_bt_test_node2 : public Condition
	{
	public:
		BEHAVIAC_DECLARE_DYNAMIC_TYPE(Condition_bt_test_node2, Condition);
		Condition_bt_test_node2()
		{
		}
	protected:
		virtual EBTStatus update_impl(Agent* pAgent, EBTStatus childStatus)
		{
			BEHAVIAC_UNUSED_VAR(pAgent);
			BEHAVIAC_UNUSED_VAR(childStatus);
			int& opl = ((FirstAgent*)pAgent)->_Get_Property_<PROPERTY_TYPE_FirstAgent_p1, int >();
			int opr = 4;
			bool op = PrivateDetails::Greater(opl, opr);
			return op ? BT_SUCCESS : BT_FAILURE;
		}
	};

	class Action_bt_test_node3 : public Action
	{
	public:
		BEHAVIAC_DECLARE_DYNAMIC_TYPE(Action_bt_test_node3, Action);
		Action_bt_test_node3()
		{
		}
	protected:
		virtual EBTStatus update_impl(Agent* pAgent, EBTStatus childStatus)
		{
			BEHAVIAC_UNUSED_VAR(pAgent);
			BEHAVIAC_UNUSED_VAR(childStatus);
			((FirstAgent*)pAgent)->SayHello();
			return BT_SUCCESS;
		}
	};

	class bt_test
	{
	public:
		static bool Create(BehaviorTree* pBT)
		{
			pBT->SetClassNameString("BehaviorTree");
			pBT->SetId((uint16_t)-1);
			pBT->SetName("test");
			pBT->SetIsFSM(false);
#if !BEHAVIAC_RELEASE
			pBT->SetAgentType("FirstAgent");
#endif
			// children
			{
				Sequence* node0 = BEHAVIAC_NEW Sequence;
				node0->SetClassNameString("Sequence");
				node0->SetId(0);
#if !BEHAVIAC_RELEASE
				node0->SetAgentType("FirstAgent");
#endif
				pBT->AddChild(node0);
				{
					Assignment_bt_test_node1* node1 = BEHAVIAC_NEW Assignment_bt_test_node1;
					node1->SetClassNameString("Assignment");
					node1->SetId(1);
#if !BEHAVIAC_RELEASE
					node1->SetAgentType("FirstAgent");
#endif
					node0->AddChild(node1);
					node0->SetHasEvents(node0->HasEvents() | node1->HasEvents());
				}
				{
					Condition_bt_test_node2* node2 = BEHAVIAC_NEW Condition_bt_test_node2;
					node2->SetClassNameString("Condition");
					node2->SetId(2);
#if !BEHAVIAC_RELEASE
					node2->SetAgentType("FirstAgent");
#endif
					node0->AddChild(node2);
					node0->SetHasEvents(node0->HasEvents() | node2->HasEvents());
				}
				{
					Action_bt_test_node3* node3 = BEHAVIAC_NEW Action_bt_test_node3;
					node3->SetClassNameString("Action");
					node3->SetId(3);
#if !BEHAVIAC_RELEASE
					node3->SetAgentType("FirstAgent");
#endif
					node0->AddChild(node3);
					node0->SetHasEvents(node0->HasEvents() | node3->HasEvents());
				}
				pBT->SetHasEvents(pBT->HasEvents() | node0->HasEvents());
			}
			return true;
		}
	};

}
#endif // _BEHAVIAC_GENERATED_BEHAVIORS_H_
