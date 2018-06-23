
export void simple(uniform int vin[], uniform int vout[], 
                   uniform int count) {
    foreach (index = 0 ... count*count) {
        int v = vin[index];
		int i = index / count;
		int j = index % count;
        vout[j*count + i] = v;
    }
}
