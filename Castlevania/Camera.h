#pragma once

class Camera final
{
public:
	Camera(float scale, float width, float height);
	~Camera() = default;

#pragma region RuleOf5
	Camera(const Camera&) = delete;
	
	Camera& operator=(const Camera&) = delete;
	
	Camera(Camera&&) = delete;
	
	Camera& operator=(Camera&&) = delete;
#pragma endregion

	void Update();

	void SetLevelBoundaries(const Rectf& levelBoundaries);
	
	void Transform(const Rectf& target);
	
	const Point2f& GetCameraBottemLeft() const;
	
	float GetScale() const;

private:
	float m_Width,
		m_Height,
		m_Scale;

	const float m_MaxScale;

	Rectf m_LevelBoundaries,
		m_FreeArea;

	Point2f m_CameraBottemLeft;

#pragma region HelperFunctions
	const Point2f& Track(const Rectf& target);
	
	void Clamp(Point2f& bottomLeftPos);
	
	void UpdareFreeArea();
	
	void ZoomIn(float value);
	
	void ZoomOut(float value);
#pragma endregion
};

