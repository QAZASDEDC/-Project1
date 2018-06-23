export uniform double simple(uniform int count) {
	uniform double sum;
	uniform double step = 1.0/(double)count;
	double x;
	double partial = 0.0;
    foreach (index = 0 ... count) {
        x = (index+0.5) * step;
		partial += step * 4.0/(1.0 + x * x);
    }
	sum = reduce_add(partial);
	return sum;
}
