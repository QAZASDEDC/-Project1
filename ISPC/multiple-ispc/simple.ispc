
export void simple(uniform int vin1[], uniform int vin2[], uniform int vout[], 
                   uniform int count) {
    foreach (index = 0 ... count*count) {
		vout[index] = 0;
		for(int k = 0; k < count; k++){
			vout[index] += vin1[index/count * count + k] * vin2[k * count + index%count];
		}

    }
}
