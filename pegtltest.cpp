#include <iostream>
#include <string>
#include <cassert>

#include <tao/pegtl.hpp>
#include "predicate.h"

#include "predicate.h"

namespace pegtl = tao::TAOCPP_PEGTL_NAMESPACE;

namespace ruleparse {

	struct and : pegtl::istring <'A', 'N', 'D' > {};

	struct notand : pegtl::until<pegtl::sor<pegtl::at<and>,pegtl::at<pegtl::eolf> > > {};

	struct net : pegtl::istring <'n', 'e', 't' > {};
	struct wildnum : pegtl::sor<pegtl::one<'*'>, pegtl::plus<pegtl::digit> > {};

	struct term : pegtl::plus<pegtl::alnum> {};
	struct ws : pegtl::plus<pegtl::space> {};

	struct ip4 : pegtl::must<wildnum, pegtl::one<'.'>, wildnum, pegtl::one<'.'>, wildnum, pegtl::one<'.'>, wildnum> {};
	struct ip4mask : pegtl::seq<pegtl::one<'/'>, pegtl::opt<ws>, pegtl::plus<pegtl::digit > > {};

	struct ipmatch : pegtl::seq < net, pegtl::plus<ws>, ip4, pegtl::opt<ws, ip4mask> > {};

	struct grammar : pegtl::must<notand, pegtl::star< and,notand > >{};

	template< typename Rule >
	struct action
		: pegtl::nothing< Rule >
	{
	};

	template<>
	struct action<ip4> {
		template <typename Input>
		static void apply(const Input& in, BpfPredicate& p) {
			std::cout << in.string();
		}
	};
	template<>
	struct action<and> {
		template <typename Input>
		static void apply(const Input& in, BpfPredicate& p) {
		//	std::cout << in.string();
		}
	};
	template<>
	struct action<notand> {
		template <typename Input>
		static void apply(const Input& in, BpfPredicate& p) {
			std::cout << in.string() << std::endl;
		}
	};

}

int main(void) {

	std::string rule = "ip proto UDP port 5060 and ether proto ip and dst net 160.96.0.0 / 12";
	//std::string rule = "bands";
	
	BpfPredicate pred;

	pegtl::string_input<> rule_in(rule, "const");
	try {
		pegtl::parse< ruleparse::grammar, ruleparse::action>(rule_in, pred);
	}
	catch(pegtl::parse_error& pe){
		std::cerr << pe.what() << std::endl;
	}

	return 0;
}