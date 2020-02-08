#include "vec.hpp"

#include <cspec.hpp>

Eval(Vec)
{
	Describe("angle()", [] {
		math::Vec vec;
		BeforeEach([&] {
			vec.P1.X = 0;
			vec.P1.Y = 0;
			vec.P2.X = 0;
			vec.P2.Y = 0;
		});

		Context("north", [&] {
			It("returns pi/2", [&] {
				vec.P1.Y = 1;
				Expect(vec.angle<double>()).toEqual(M_PI_2);
			});
		});

		Context("very north", [&] {
			It("returns pi/2", [&] {
				vec.P1.Y = 100;
				Expect(vec.angle<double>()).toEqual(M_PI_2);
			});
		});

		Context("west", [&] {
			It("returns pi", [&] {
				vec.P1.X = -1;
				Expect(vec.angle<double>()).toEqual(M_PI);
			});
		});

		Context("south", [&] {
			It("returns 3pi/2", [&] {
				vec.P1.Y = -1;
				Expect(vec.angle<double>()).toEqual(3.0 * M_PI / 2.0);
			});
		});

		Context("east", [&] {
			It("returns 0", [&] {
				vec.P1.X = 1;
				Expect(vec.angle<double>()).toEqual(0.0);
			});
		});
	});
}
