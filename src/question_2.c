// TODO: Is there some way to take my existing 1d ising code and make
// that work in two dimensions without having to re-write it. The answer is 
// yes and it just comes down to correct determination of the neighbours 
// and then I can just use it out of the box. In other words I just need 
// to overide the spin_energy method. 

// The other option is of course just to re-write all of the code here. 
// Maybe that will be the simplest. 
// How do I find the edges?

// i % 5:   00, 01, 02, 03, 04  (i / 5)
//
//          00, 01, 02, 03, 04  00
//          05, 06, 07, 08, 09  01
//          10, 11, 12, 13, 14  02
//          15, 16, 17, 18, 19  03
//          20, 21, 22, 23, 24  04
// 
// Hence we are on a boundary if (i / num) == 0 or (i / num) == (num - 1)
// or (i % num) == 0 or (i % num) == (num - 1). Now consider the neighbours
//                              
//          000, 001, 002, 003, *04 
//          005, 006, 007, *08, *09 
//          010, 011, 012, 013, *14 
//          015, 016, 017, 018, 019 
//          020, 021, 022, 023, 024 
//
// So in general the neighbours are (spin - num) , (spin + num). These are the 
// above and below. Then the left and right neighbours are just (spin + 1)
// and (spin - 1).
//
// If we are on the left boundary (i.e. (i % num == 0) then I need to consider:
//  1. (spin + num),
//  2. (spin - num), 
//  3. (spin + 1),
//  4. (spin + num - 1).
// If I am on the right boundary (i.e. (i % num == (num - 1)) then I need to 
// consider:
//  1. (spin + num)
//  2. (spin - num)
//  3. (spin - 1)
//  4. (spin - num + 1)
// If I am on the top boundary (i.e. (i / num == 0)) then I need to consider:
//  1. (spin + num)
//  2. (num * (num - 1) + spin)
//  3. (spin + 1)
//  4. (spin - 1)
// Finally if I am on the bottom boundary (i.e. (i / num == (num - 1)) then
// I need to consider:
//  1. (spin - num)
//  2. (spin - num * (num - 1))
//  3. (spin + 1)
//  4. (spin - 1)
//
// OK it should work perfectly with just this change to the spin energy 
// calculation.
//
// Hang on I need to work out corners. These are the squares where both 
// (i / num == 0 and i % num == 0) or 
// (i / num == 0 and i % num == (num - 1)) or
// (i / num == (num - 1) and i % num == 0) or 
// (i / num == (num - 1) and i % num == (num - 1)). Alternatively,
// They should be calculatable in advance as 0, num - 1, num * (num - 1) and 
// num * num - 1
// 
// At the top left corner (i.e. spin == 0) I need to consider:
//  1. (spin + 1) == 1
//  2. (spin + num) == num
//  3. (num * (num - 1))
//  4. (num - 1)
int spin_energy(int spin, int spins[], int num_spins)
{
    int num = sqrt(num_spins);
    int energy = 0;

    switch (spin % num)
    {
        case 0: 
        {
            energy += spins[spin] * (spins[spin + 1] + spins[spin + num - 1]);
        }
        case num - 1: 
        {
            energy += spins[spin] * (spins[spin - 1] + spins[spin - num + 1]);
        }
        default:
        {
            energy += spins[spin] * (spins[spin + num] + spins[spin - num]);
        }
    }

    switch ((int) spin / num)
    {
        case 0:
        {
            energy += spins[spin] * (spins[spin + num] + 
                spins[spin + num * (num - 1)]);
        }
        case num - 1:
        {
            energy += spins[spin] * (spins[spin - num] + 
                spins[spin - num * (num - 1)]) 
        }
        default:
        {
            energy += spins[spin] * (spins[spin - num] + spins[spin + num]);
        }
    }

    return energy;
}



/*
 * question_2_a
 * ------------
 * Consider a 100 by 100 2D ising model and provide plots of the 
 * initial and final states for at least three different temperatures.
 */
