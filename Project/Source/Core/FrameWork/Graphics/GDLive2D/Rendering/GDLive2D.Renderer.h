#include "Rendering/CubismRenderer.hpp"
#include "CubismFramework.hpp"
#include "Type/csmVector.hpp"
#include "Type/csmRectF.hpp"
#include "Math/CubismVector2.hpp"
#include "Type/csmMap.hpp"

#include <RID.hpp>

using namespace Live2D::Cubism::Framework;

namespace GDLive2D
{
	class CubismClippingManager
	{
	private:
		Rendering::CubismRenderer::CubismTextureColor* GetChannelFlagAsColor(csmInt32 channelNo);
		void CalcClippedDrawTotalBounds(CubismModel& model, CubismClippingContext* clippingContext);

		CubismClippingManager();
		virtual ~CubismClippingManager();

		void Initialize(CubismModel& model, csmInt32 drawableCount, const csmInt32** drawableMasks, const csmInt32* drawableMaskCounts);

		void SetupClippingContext(CubismModel& model, godot::RID m_canvas);

		CubismClippingContext* FindSameClip(const csmInt32* drawableMasks, csmInt32 drawableMaskCounts) const;

		void SetupLayoutBounds(csmInt32 usingClipCount) const;

		csmVector<CubismClippingContext*>* GetClippingContextListForDraw();

		void SetClippingMaskBufferSize(csmFloat32 width, csmFloat32 height);

		CubismVector2 GetClippingMaskBufferSize() const;

		csmInt32    _currentFrameNo;

		csmVector<Rendering::CubismRenderer::CubismTextureColor*>  _channelColors;
		csmVector<CubismClippingContext*>               _clippingContextListForMask;
		csmVector<CubismClippingContext*>               _clippingContextListForDraw;
		CubismVector2                                   _clippingMaskBufferSize;

		CubismMatrix44  _tmpMatrix;
		CubismMatrix44  _tmpMatrixForMask;
		CubismMatrix44  _tmpMatrixForDraw;
		csmRectF        _tmpBoundsOnModel;
	};

	class CubismClippingContext
	{
	private:
		CubismClippingContext(CubismClippingManager* manager, const csmInt32* clippingDrawableIndices, csmInt32 clipCount);
		virtual ~CubismClippingContext();

		void AddClippedDrawable(csmInt32 drawableIndex);

		CubismClippingManager* GetClippingManager();

		csmBool _isUsing;
		const csmInt32* _clippingIdList;
		csmInt32 _clippingIdCount;
		csmInt32 _layoutChannelNo;
		csmRectF* _layoutBounds;
		csmRectF* _allClippedDrawRect;
		CubismMatrix44 _matrixForMask;
		CubismMatrix44 _matrixForDraw;
		csmVector<csmInt32>* _clippedDrawableIndexList;

		CubismClippingManager* _owner;
	};

	class CubismRenderer : public Rendering::CubismRenderer
	{
	public:

	protected:

	private:

	};
}