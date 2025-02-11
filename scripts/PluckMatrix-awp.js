/* Declares the PluckMatrix Audio Worklet Processor */

class PluckMatrix_AWP extends AudioWorkletGlobalScope.WAMProcessor
{
  constructor(options) {
    options = options || {}
    options.mod = AudioWorkletGlobalScope.WAM.PluckMatrix;
    super(options);
  }
}

registerProcessor("PluckMatrix", PluckMatrix_AWP);
