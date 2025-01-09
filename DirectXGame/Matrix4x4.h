namespace KamataEngine {

/// <summary>
/// 4x4�s��
/// </summary>
struct Matrix4x4 final {
	float m[4][4];

	// *= ���Z�q�̃I�[�o�[���[�h
	Matrix4x4& operator*=(const Matrix4x4& rhs) {
		*this = *this * rhs;
		return *this;
	}

	// �s��̊|���Z�̃I�[�o�[���[�h
	Matrix4x4 operator*(const Matrix4x4& rhs) const {
		Matrix4x4 result = {}; // ������
		for (int i = 0; i < 4; ++i) {
			for (int j = 0; j < 4; ++j) {
				result.m[i][j] = 0.0f;
				for (int k = 0; k < 4; ++k) {
					result.m[i][j] += m[i][k] * rhs.m[k][j];
				}
			}
		}
		return result;
	}

};

} // namespace KamataEngine
