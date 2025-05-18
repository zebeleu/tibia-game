#ifndef TIBIA_MATRIX_HH_
#define TIBIA_MATRIX_HH_ 1

typedef<typename T>
struct matrix{
	// REGULAR FUNCTIONS
	// =========================================================================
	matrix(int xmin, int xmax, int ymin, int ymax){
		int dx = (xmax - xmin) + 1;
		int dy = (ymax - ymin) + 1;

		if(dx < 1 || dy < 1){
			error("matrix: Ungueltige Feldgroesse %d..%d, %d..%d.\n", xmin, xmax, ymin, ymax);

			if(dx < 1){
				dx = 1;
			}

			if(dy < 1){
				dy = 1;
			}
		}

		this->xmin = xmin;
		this->ymin = ymin;
		this->dx = dx;
		this->dy = dy;
		this->entry = new T[dx * dy];
	}

	matrix(int xmin, int xmax, int ymin, int ymax, T init) : matrix(xmin, xmax, ymin, ymax) {
		int count = this->dx * this->dy;
		for(int i = 0; i < count; i += 1){
			this->entry[i] = init;
		}
	}

	// TODO(fusion): Probably missing some inlined destructor?

	T *at(int x, int y){
		int xoffset = x - this->xmin;
		int yoffset = y - this->ymin;
		if(xoffset < 0 || xoffset >= this->dx || yoffset < 0 || yoffset >= this->dy){
			error("matrix::operator(): Ungueltiger Index %d/%d.\n", x, y);
			return &this->entry[0];
		}else{
			// TODO(fusion): Are we really storing this in row major order?
			return &this->entry[yoffset * this->dx + xoffset];
		}
	}

	// DATA
	// =========================================================================
	int xmin;
	int ymin;
	int dx;
	int dy;
	T *entry;
};

typedef<typename T>
struct matrix3d{
	// REGULAR FUNCTIONS
	// =========================================================================
	matrix3d(int xmin, int xmax, int ymin, int ymax, int zmin, int zmax){
		int dx = (xmax - xmin) + 1;
		int dy = (ymax - ymin) + 1;
		int dz = (zmax - zmin) + 1;

		if(dx < 1 || dy < 1 || dz < 1){
			error("matrix3d: Ungueltige Feldgroesse %d..%d, %d..%d, %d..%d.\n",
					xmin, xmax, ymin, ymax, zmin, zmax);

			if(dx < 1){
				dx = 1;
			}

			if(dy < 1){
				dy = 1;
			}

			if(dz < 1){
				dz = 1;
			}
		}

		this->xmin = xmin;
		this->ymin = ymin;
		this->dx = dx;
		this->dy = dy;
		this->entry = new T[dx * dy];
	}

	matrix3d(int xmin, int xmax, int ymin, int ymax, int zmin, int zmax, T init)
			: matrix(xmin, xmax, ymin, ymax, zmin, zmax) {
		int count = this->dx * this->dy * this->dz;
		for(int i = 0; i < count; i += 1){
			this->entry[i] = init;
		}
	}

	// TODO(fusion): Probably missing some inlined destructor?

	T *at(int x, int y, int z){
		int xoffset = x - this->xmin;
		int yoffset = y - this->ymin;
		int zoffset = z - this->zmin;
		if(xoffset < 0 || xoffset >= this->dx
				|| yoffset < 0 || yoffset >= this->dy
				|| zoffset < 0 || zoffset >= this->dz){
			error("matrix3d::operator(): Ungueltiger Index %d/%d/%d.\n", x, y, z);
			return &this->entry[0];
		}else{
			// TODO(fusion): Same as `matrix::at` on the XY plane.
			return &this->entry[zoffset * this->dx * this->dy
								+ yoffset * this->dx
								+ xoffset];
		}
	}

	// DATA
	// =========================================================================
	int xmin;
	int ymin;
	int zmin;
	int dx;
	int dy;
	int dz;
	T *entry;
};

#endif //TIBIA_MATRIX_HH_
