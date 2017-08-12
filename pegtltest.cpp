#include <iostream>
#include <string>
#include <cassert>
#include <fstream>

#include <tao/pegtl.hpp>
#include "predicate.h"

namespace pegtl = tao::TAOCPP_PEGTL_NAMESPACE;

struct state {
	enum BPFdir dir;
	enum BPFproto proto;
	enum BPFtype type;
	void clear() { dir = BPFdirOR; proto = BPFprotoAll; type = BPFtypeHost; }
	state() { clear(); }
};

namespace ruleparse {

	struct and : pegtl::istring <'A', 'N', 'D' > {};
	struct ws : pegtl::plus<pegtl::space> {};
	struct notand : pegtl::until<pegtl::sor<pegtl::at<and>,pegtl::at<pegtl::eolf> > > {};
	struct grammar : pegtl::must<notand, pegtl::star< and, notand > > {};

	struct wildnum : pegtl::sor<pegtl::one<'*'>, pegtl::plus<pegtl::digit> > {};

	struct dst : pegtl::istring <'d', 's', 't' > {};
	struct src : pegtl::istring <'s', 'r', 'c' > {};
	struct net : pegtl::istring <'n', 'e', 't' > {};
	struct ip : pegtl::istring <'i', 'p'  > {};
	struct ip6 : pegtl::istring <'i', 'p', '6' > {};
	struct arp : pegtl::istring <'a', 'r', 'p' > {};
	struct rarp : pegtl::istring <'r', 'a', 'r', 'p' > {};
	struct tcp : pegtl::istring <'t', 'c', 'p' > {};
	struct udp : pegtl::istring <'u', 'd', 'p' > {};
	struct mpls : pegtl::istring <'m', 'p', 'l', 's' > {};
	struct icmp : pegtl::istring <'i', 'c', 'm', 'p' > {};
	struct vlan : pegtl::istring <'v', 'l', 'a', 'n' > {};
	struct proto : pegtl::istring<'p', 'r', 'o', 't', 'o'> {};
	struct ether : pegtl::istring<'e', 't', 'h', 'e', 'r'> {};
	struct port : pegtl::istring<'p', 'o', 'r', 't'> {};

//check for src or dst
	struct srcdst : pegtl::opt <pegtl::sor <src, dst>, pegtl::plus<ws> > {};

//match against port
	struct portnum : pegtl::plus<pegtl::digit> { };
	struct portmatch : pegtl::seq<srcdst, port,ws, portnum > {};

//match net addr
	struct ip4 : pegtl::must<wildnum, pegtl::one<'.'>, wildnum, pegtl::one<'.'>, wildnum, pegtl::one<'.'>, wildnum> {};
	struct ip4masknum : pegtl::plus<pegtl::digit> {};
	struct ip4mask : pegtl::seq<pegtl::one<'/'>, pegtl::opt<ws>, ip4masknum > {};
	struct netmatch : pegtl::seq<srcdst, net, ws, ip4, pegtl::opt<ws, ip4mask> >{};

//match ether proto
	struct eproto : pegtl::sor<ip, ip6, arp, rarp, mpls, vlan> {};
	struct eprotomatch : pegtl::seq<ether, ws, proto, ws, eproto> {};

//match ip proto
	struct iproto : pegtl::sor<tcp, udp, icmp> {};
	struct iprotomatch : pegtl::seq<ip, ws, proto, ws, iproto> {};

//matchany
	struct matchable : pegtl::sor <iprotomatch, eprotomatch, netmatch, portmatch, and> {};
	struct findmatch : pegtl::must < pegtl::plus <matchable, pegtl::opt<ws> > > {};

	//struct ipmatchprefix : pegtl::until < pegtl::sor<pegtl::at<src>, pegtl::at<net>, pegtl::at<dst>>> {};
	struct ipmatchprefix : pegtl::until < pegtl::sor<pegtl::at<src>, pegtl::at<net>, pegtl::at<dst>>> {};
	struct ipmatchold : pegtl::seq <ipmatchprefix,pegtl::opt <pegtl::sor<src,dst> >, pegtl::plus<ws>, net, pegtl::plus<ws>, ip4, pegtl::opt<ws, ip4mask> > {};

	template< typename Rule >
	struct action
		: pegtl::nothing< Rule >
	{
	};

	template<>
	struct action<portnum> {
		template <typename Input>
		static void apply(const Input& in, BpfPredicate& p, struct state& st) {
//			if (st.dir == BPFdirAND || st.dir == BPFdirDEST) p.addDestPort(in.string());
//			if (st.dir == BPFdirAND || st.dir == BPFdirSRC) p.addSrcPort(in.string());
			if (st.dir == BPFdirAND || st.dir == BPFdirOR) {
				p.addPort(in.string());
				p.dir = st.dir;
			}
			else if (st.dir == BPFdirSRC) p.addSrcPort(in.string());
			else if (st.dir == BPFdirDEST) p.addDestPort(in.string());
		}
	};
	template<>
	struct action<iproto> {
		template <typename Input>
		static void apply(const Input& in, BpfPredicate& p, struct state& st) {
			std::cout << "set iproto to " << in.string() << std::endl;
		}
	};
	template<>
	struct action<eproto> {
		template <typename Input>
		static void apply(const Input& in, BpfPredicate& p, struct state& st) {
			std::cout << "set eproto to " << in.string() << std::endl;
		}
	};
	template<>
	struct action<src> {
		template <typename Input>
		static void apply(const Input& in, BpfPredicate& p, struct state& st) {
			st.dir = BPFdirSRC;
		}
	};
	template<>
	struct action<dst> {
		template <typename Input>
		static void apply(const Input& in, BpfPredicate& p, struct state& st) {
			st.dir = BPFdirDEST;
		}
	};
	template<>
	struct action<ip4> {
		template <typename Input>
		static void apply(const Input& in, BpfPredicate& p, struct state& st) {
			std::cout << in.string();
			if (st.dir == BPFdirAND || st.dir == BPFdirDEST) p.addDestIP4addr(in.string());
			if (st.dir == BPFdirAND || st.dir == BPFdirSRC) p.addSrcIP4addr(in.string());
		}
	};
	template<>
	struct action<ip4masknum> {
		template <typename Input>
		static void apply(const Input& in, BpfPredicate& p, struct state& st) {
			std::cout << in.string();
			if (st.dir == BPFdirAND || st.dir == BPFdirDEST) p.addDestIP4mask(in.string());
			if (st.dir == BPFdirAND || st.dir == BPFdirSRC) p.addSrcIP4mask(in.string());
		}
	};
	template<>
	struct action<and> {
		template <typename Input>
		static void apply(const Input& in, BpfPredicate& p, struct state& st) {
			st.clear();
		//	std::cout << in.string();
		}
	};
	template<>
	struct action<notand> {
		template <typename Input>
		static void apply(const Input& in, BpfPredicate& p, struct state& st) {
			std::cout << in.string() << std::endl;
		}
	};

}

int main(void) {

	std::string rule = "ip proto UDP port 5060 and ether proto ip and dst net 160.96.0.0 / 12";
	struct state st;
	//std::string rule = "bands";
	
	BpfPredicate pred;

	pegtl::string_input<> rule_in(rule, "const");
	try {
		//pegtl::parse< ruleparse::ipmatchold, ruleparse::action>(rule_in, pred, st);
		pegtl::parse< ruleparse::findmatch, ruleparse::action>(rule_in, pred, st);
	}
	catch(pegtl::parse_error& pe){
		std::cerr << pe.what() << std::endl;
	}

	return 0;
}