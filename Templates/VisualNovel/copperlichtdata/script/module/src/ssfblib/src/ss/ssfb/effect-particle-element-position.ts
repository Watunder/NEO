// automatically generated by the FlatBuffers compiler, do not modify

import * as flatbuffers from 'flatbuffers';

export class EffectParticleElementPosition {
  bb: flatbuffers.ByteBuffer|null = null;
  bb_pos = 0;
  __init(i:number, bb:flatbuffers.ByteBuffer):EffectParticleElementPosition {
  this.bb_pos = i;
  this.bb = bb;
  return this;
}

offsetXminValue():number {
  return this.bb!.readFloat32(this.bb_pos);
}

offsetXmaxValue():number {
  return this.bb!.readFloat32(this.bb_pos + 4);
}

offsetYminValue():number {
  return this.bb!.readFloat32(this.bb_pos + 8);
}

offsetYmaxValue():number {
  return this.bb!.readFloat32(this.bb_pos + 12);
}

static sizeOf():number {
  return 16;
}

static createEffectParticleElementPosition(builder:flatbuffers.Builder, OffsetXMinValue: number, OffsetXMaxValue: number, OffsetYMinValue: number, OffsetYMaxValue: number):flatbuffers.Offset {
  builder.prep(4, 16);
  builder.writeFloat32(OffsetYMaxValue);
  builder.writeFloat32(OffsetYMinValue);
  builder.writeFloat32(OffsetXMaxValue);
  builder.writeFloat32(OffsetXMinValue);
  return builder.offset();
}

}