import { dlopen, FFIType, ptr, toArrayBuffer } from 'bun:ffi'

const { symbols: dylib } = dlopen('bindings.dylib', {
  createBuffer: {
    args: [FFIType.i64],
    returns: FFIType.ptr
  },
  dispose: {
    args: [FFIType.ptr]
  },
  destroyBuffer: {
    args: [FFIType.ptr, FFIType.ptr]
  },
  genBufferDestroyer: {
    returns: FFIType.ptr
  }
})

class C {
  constructor(size) {
    const _ptr = dylib.createBuffer(size)
    this.underlying = toArrayBuffer(
      _ptr,
      0,
      size * 4,
      dylib.genBufferDestroyer.native()
    )
  }

  dispose() {
    dylib.dispose(ptr(this.underlying))
  }
}

for (let i = 0; i < 1000; ++i) {
  const c = new C(1)
  c.dispose()
  Bun.gc(true)
}
