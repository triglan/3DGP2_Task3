#pragma once
#include "GameObject.h"
#include "MouseUtil.h"
#include "CameraUtil.h"
#include <cmath>

#include "LineBrush.h"
#include "Mode0.h"
#include "Mode1.h"
#include "TimerUtil.h"

class Enemy : public GameObject {
private:
	bool MoveForward{}, MoveBackward{}, MoveRight{}, MoveLeft{ true };
	float WingRotation{ 90 };
	XMFLOAT3 HeliRotation{ 0, 180, 0 };
	XMFLOAT3 Tilt{ 0, 0, 0 };
	
public:
	XMFLOAT3 Position{ 0.0, 0.0, 100.0 };
	XMFLOAT3 Rotation{};
	XMFLOAT3 RotationDest{};
	XMFLOAT3 Size{ 0.5, 0.5, 0.5 };
	float GunAlpha = 1.0;

	bool UseLight{};

	LineBrush line;

	OOBB oobb;
	Range range;
	//OOBB GetRANGE;
	Range GetRange() { return range; }
	Vector Vec{};
	bool GetIsCol{};

	Enemy() {
		Math::InitVector(Vec);
		line.SetColor(1.0, 1.0, 1.0);
	}

	XMFLOAT3 GetUp() {
		return Vec.Up;
	}

	XMFLOAT3 GetLook() {
		return Vec.Look;
	}

	XMFLOAT3 GetRight() {
		return Vec.Right;
	}

	XMFLOAT3 GetPosition() {
		return Position;
	}


	void Update(float FT) {
		if (Position.x > 20) {
			MoveLeft = true;
			MoveRight = false;
		}
		else if (Position.x < -20) {
			MoveLeft = false;
			MoveRight = true;
		}
		//Move
		if (MoveForward) {
			Position.z += FT * 10;
		}
		if (MoveBackward) {
			Position.z -= FT * 10;
		}
		if (MoveRight) {
			Position.x += FT * 10;
		}
		if (MoveLeft) {
			Position.x -= FT * 10;
		}


		//날개 회전
		WingRotation += FT * 2000;

	}

	void Render(CommandList CmdList) {
		//헬기 몸통
		InitMatrix(CmdList, RENDER_TYPE_PERS);

		Transform::Scale(ScaleMatrix, 0.5, 0.5, 0.5);
		Transform::Move(TranslateMatrix, Position.x, Position.y, Position.z);

		Transform::Rotate(TranslateMatrix, Tilt.x, HeliRotation.y, Tilt.z);
		Transform::Rotate(TranslateMatrix, HeliRotation.x, 0.0, 0.0);
		Transform::Rotate(TranslateMatrix, 0, 0.0, HeliRotation.z);

		FlipTexture(CmdList, FLIP_TYPE_V); 
		RenderMesh(CmdList, HelicopterBodyMesh, HelicopterTex, ObjectShader);

		//헬기 머리
		Transform::Move(TranslateMatrix, 0.0, 2.0, 0.0);
		Transform::Rotate(TranslateMatrix, 0.0, WingRotation, 0.0);
		RenderMesh(CmdList, HelicopterHeadMesh, HelicopterTex, ObjectShader);

		// 바운드 스페어 출력
		range.Update(Position, 10);
		//range.Render(CmdList);

		Math::UpdateVector(Vec, HeliRotation.x, HeliRotation.y, HeliRotation.z);



	}
};