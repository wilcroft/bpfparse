#include <iostream>
#include <string>
#include <cassert>

#include <tao/pegtl.hpp>

#include "predicate.h"

namespace pegtl = tao::TAOCPP_PEGTL_NAMESPACE;

namespace rule {

	struct and : pegtl::istring < ' ', 'a', 'n', 'd', ' ' > {};
	struct or : pegtl::istring < ' ', 'o', 'r', ' ' > {};

	struct prim {
		enum BPFprimatives p;
		int v;
	};


	struct primatives {
		primatives()
		{
			// By default we initialise with all binary operators from the C language that can be
			// used on integers, all with their usual priority.

			insert("dst", BPFprimDir, BPFdirDEST);
			insert("src", BPFprimDir, BPFdirSRC);
			insert("ether", BPFprimProto, BPFprotoEther);
			insert("net", BPFprimProto, BPFdirDEST);

			//missing


		}

		// Arbitrary user-defined operators can be added at runtime.

		void insert(const std::string& name, const enum BPFprimatives p, const int v)
		{
			assert(!name.empty());
			m_ops.insert({ name,{ p, v } });
		}

		const std::map< std::string, prim >& ops() const
		{
			return m_ops;
		}

	private:
		std::map< std::string, prim > m_ops;
	};

	// The actions for the brackets call functions that create, and collect
	// a temporary additional stack for evaluating the bracketed expression.

	// Class that takes care of an operand and an operator stack for
	// shift-reduce style handling of operator priority; in a
	// reduce-step it calls on the functions contained in the op
	// instances to perform the calculation.

	struct stack
	{
		stack() {
			m_p.emplace_back();
		}
		void push(const BpfPredicate& b)
		{
			m_p.push_back(b);
		}

		void push(const long l)
		{
			m_l.push_back(l);
		}

		long finish()
		{
			while (!m_p.empty()) {
				reduce();
			}
			assert(m_l.size() == 1);
			const auto r = m_l.back();
			m_l.clear();
			return r;
		}

	private:
		std::vector< BpfPredicate > m_p;
		std::vector< long > m_l;

		void reduce()
		{
//			assert(!m_o.empty());
//			assert(m_l.size() > 1);

//			const auto r = m_l.back();
//			m_l.pop_back();
//			const auto l = m_l.back();
//			m_l.pop_back();
//			const auto o = m_o.back();
//			m_o.pop_back();
//			m_l.push_back(o.f(l, r));
		}
	};

	// Additional layer, a "stack of stacks", to clearly show how bracketed
	// sub-expressions can be easily processed by giving them a stack of
	// their own. Once a bracketed sub-expression has finished evaluation on
	// its stack, the result is pushed onto the next higher stack, and the
	// sub-expression's temporary stack is discarded. The top-level calculation
	// is handled just like a bracketed sub-expression, on the first stack pushed
	// by the constructor.

	struct stacks
	{
		stacks()
		{
			open();
		}

		void open()
		{
			m_v.emplace_back();
		}

		template< typename T >
		void push(const T& t)
		{
			assert(!m_v.empty());
			m_v.back().push(t);
		}

		void close()
		{
			assert(m_v.size() > 1);
			const auto r = m_v.back().finish();
			m_v.pop_back();
			m_v.back().push(r);
		}

		long finish()
		{
			assert(m_v.size() == 1);
			return m_v.back().finish();
		}

	private:
		std::vector< stack > m_v;
	};

	template< typename Rule >
	struct action
		: pegtl::nothing< Rule >
	{
	};


	template<>
	struct action< pegtl::one< '(' > >
	{
		static void apply0(stacks& s)
		{
			s.open();
		}
	};

	template<>
	struct action< pegtl::one< ')' > >
	{
		static void apply0(stacks& s)
		{
			s.close();
		}
	};

	template<>
	struct action < and > {

		static void newPred(stack& s) {

		}
	};


}