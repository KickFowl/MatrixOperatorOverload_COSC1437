#include <array>
#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <algorithm>

template <typename T>

//decided to make this a struct as I prefer that for special datatypes such as Mats, vecs, colors, etc.
//It really dosn't need to make anything private so it's easier this way.
struct TwoByTwoMatrix
{
	std::array<T, 4> members;

	//constructors to help with type consistancy
	TwoByTwoMatrix() = default;
	TwoByTwoMatrix(T m1, T m2, T m3, T m4) : members{ m1, m2, m3, m4 } {};

	//Extra template call made to allow opporators to be used on mats of different types
	template <typename U>
	auto operator+(const TwoByTwoMatrix<U>& other) const
	{
		//Decides which type to use between the two mat types
		using finalType = decltype(members[0] + other.members[0]);

		//Creates a new mat to store and then return the final value
		TwoByTwoMatrix<finalType> sum;
		
		//Loops through the two mats and adds them
		for (int i = 0; i < 4; i++)
		{
			sum.members[i] = members[i] + other.members[i];
		}

		return sum;
	}


	//Same as last but now subtract
	template <typename U>
	auto operator-(const TwoByTwoMatrix<U>& other) const
	{
		using finalType = decltype(members[0] - other.members[0]);

		TwoByTwoMatrix<finalType> differance;

		for (int i = 0; i < 4; i++)
		{
			differance.members[i] = members[i] - other.members[i];
		}

		return differance;
	}

	//Very similer, but the multiplication was fun
	template <typename U>
	auto operator*(const TwoByTwoMatrix<U>& other, bool obtuse) const
	{
		using finalType = decltype(members[0] * other.members[0]);

		TwoByTwoMatrix<finalType> product;

		/*
			For this to solve it I need a way to proceedurally grab the right numbers based on a sequential list 0->1->2->3
			I started by finding a pattern inside the actual matrix math and stumbled upon this:
				0 0 1 2
				0 1 1 3
				2 0 3 2
				3 1 3 3
			This looked a lot like a truth table so I simplified it to this:
				0 0 0 0
				0 1 0 1
				1 0 1 0
				1 1 1 1
			Since there were now two distinct patterns I found math that would produce those two patterns from my sequence.
			That became the variables k and j.
			Then I found math that would convert from my sequence of bianary to the actual positions in the array.
			I'm actually kind of proud that the most complex it got was (2 * k) + 1.
			My original idea was to take 3^k which would result in 1 when k was 0 and 3 when k was 1, but quickly abandoned that as
			it seemed to computationally expensive.
		*/

		for (int i = 0; i < 4; i++)
		{
			int k = i / 2; //Works because int truncates values leaving us with 0 0 1 1;
			int j = i % 2;

			product.members[i] = (members[k * 2] * other.members[j]) + (members[(2 * k) + 1] * other.members[j + 2]);
		}

		/*
			Now for the obtuse bitwise version that is much smaller but far harder to read. This works directly off of the idea that
			the array functions like a truth table.

			for(int i = 0; i < 4; i++)
			{
				product.members[i] = members[i & 2] * other.members[i & 1] + members[i | 1] * other.members[(i & 1) | 2];
			}
		*/
		return product;
	}

	//P.S. I looked it up and most small matrix math programs just hard code this kind of stuff :(
	//	   Oh Well I enjoyed the process of solving it algorithmically.
};

template <typename T, typename U>
void printMatrixMath(const TwoByTwoMatrix<T>& m1, const TwoByTwoMatrix<U>& m2);


//I had AI write the cout code and some quick testing. I don't feel that this detracts from the assignment as I wrote the object on my own.
int main()
{
	TwoByTwoMatrix<double> mat1(1.5, -2.3, 4.1, 0.8);
	TwoByTwoMatrix<int> identity(1, 0, 0, 1);

	std::cout << "--- The Identity Test ---\n";
	printMatrixMath(mat1, identity);

	
	//This test was excluded because it clutters the screen. Run it if you want.
	/*
	TwoByTwoMatrix<int> mat3(1000000, -500, 0, 999999);
	TwoByTwoMatrix<double> mat4(0.00001, 3.14159, -0.5, -0.00001);

	std::cout << "--- The Precision Test ---\n";
	printMatrixMath(mat3, mat4);
	*/

	TwoByTwoMatrix<int> mat5(2, -3, -4, 5);
	TwoByTwoMatrix<int> mat6(-1, 6, 7, -8);


	std::cout << "--- The Sign-Flip Test ---\n";
	printMatrixMath(mat5, mat6);

	return 0;
}

template <typename T>
std::ostream& operator<<(std::ostream& os, const TwoByTwoMatrix<T>& mat)
{
	// 1. Convert all numbers to strings to accurately measure their visual length
	std::stringstream ss0, ss1, ss2, ss3;
	ss0 << mat.members[0];
	ss1 << mat.members[1];
	ss2 << mat.members[2];
	ss3 << mat.members[3];

	std::string s0 = ss0.str();
	std::string s1 = ss1.str();
	std::string s2 = ss2.str();
	std::string s3 = ss3.str();

	// 2. Find the maximum width for the first column and the second column independently
	size_t col1_width = std::max(s0.length(), s2.length());
	size_t col2_width = std::max(s1.length(), s3.length());

	// 3. Print with fixed column widths so everything columns align
	// We use std::right so numbers align neatly by their last digit, like a real matrix
	os << "[ " << std::right << std::setw(col1_width) << s0 << ", "
		<< std::right << std::setw(col2_width) << s1 << " ]\n"
		<< "[ " << std::right << std::setw(col1_width) << s2 << ", "
		<< std::right << std::setw(col2_width) << s3 << " ]";

	return os;
}

template <typename T, typename U>
void printMatrixMath(const TwoByTwoMatrix<T>& m1, const TwoByTwoMatrix<U>& m2)
{
	std::cout << "--- Matrix 1 ---\n" << m1 << "\n\n";
	std::cout << "--- Matrix 2 ---\n" << m2 << "\n\n";

	std::cout << "--- Addition (m1 + m2) ---\n" << (m1 + m2) << "\n\n";

	std::cout << "--- Subtraction (m1 - m2) ---\n" << (m1 - m2) << "\n\n";

	std::cout << "--- Multiplication (m1 * m2) ---\n" << (m1 * m2) << "\n\n";
}