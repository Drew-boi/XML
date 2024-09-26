

//    bitset<32> bid(emprec.id);
//    int value = static_cast<int>(bid.to_ulong());
//    cout << bid << endl;
//    os.write(reinterpret_cast<const char*>(&value), sizeof(value)); // Extra work just to get it back to an int, still is cool use of bitset and reinterpret_cast.
//    bitset<248> bname;
//    for (int i = 0; i < 31; ++i) {
//        char c = emprec.name[i];
//        for (int j = 7; j >= 0; --j) {
//            if (c & 0x1) {
//                bname.set(8 * i + (7 - j));
//            }
//            c >>= 1;
//        }
//    } // cool way of converting a char to bitset.
//
//    doublelong d; // use a union so you can pass the double bits as a long to bitset
//    d.d = emprec.salary;
//    bitset<64> bsalary(d.u); // more cool unneeded stuff
