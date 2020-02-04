#include "math.hpp"

#include <cspec.hpp>

Eval(GCD)
{
	Describe("x is 0", [] {
		It("returns y", [] {
			Expect(math::GCD(0u, 13u)).toEqual(13u);
		});
	});

	Describe("y is 0", [] {
		It("returns x", [] {
			Expect(math::GCD(13u, 0u)).toEqual(13u);
		});
	});

	Describe("x & y are 0", [] {
		It("returns 0", [] {
			Expect(math::GCD(0u, 0u)).toEqual(0u);
		});
	});

	Describe("100 & 10", [] {
		It("returns 10", [] {
			Expect(math::GCD(100u, 10u)).toEqual(10u);
		});
	});

	Describe("10 & 100", [] {
		It("returns 10", [] {
			Expect(math::GCD(10u, 100u)).toEqual(10u);
		});
	});

	Describe("270 & 3830", [] {
		It("returns 10", [] {
			Expect(math::GCD(270u, 3830u)).toEqual(10u);
		});
	});

	Describe("384 & 512", [] {
		It("returns 128", [] {
			Expect(math::GCD(384u, 512u)).toEqual(128u);
		});
	});
}

Eval(Frac)
{
	Describe("0.5", [] {
		It("returns 1 / 2", [] {
			auto actual = math::Frac(0.5);
			auto expected = std::make_tuple(1u, 2u);
			Expect(std::get<0>(actual)).toEqual(std::get<0>(expected));
			Expect(std::get<1>(actual)).toEqual(std::get<1>(expected));
		});
	});

	Describe("3.0", [] {
		It("returns 3 / 1", [] {
			auto actual = math::Frac(3.5);
			auto expected = std::make_tuple(3u, 1u);
			Expect(std::get<0>(actual)).toEqual(std::get<0>(expected));
			Expect(std::get<1>(actual)).toEqual(std::get<1>(expected));
		});
	});
}
