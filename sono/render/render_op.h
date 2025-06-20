#ifndef SN_RENDER_OP_H
#define SN_RENDER_OP_H

/// abstracting primitive render operation for submitting to render queue
class RenderOp {
public:
  enum RenderOpCode { ROP_TRIANGLES, ROP_LINE_STRIP };
  RenderOpCode rop;
};

#endif // !SN_RENDER_OP_H
