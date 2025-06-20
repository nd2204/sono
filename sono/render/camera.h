#ifndef CAMERA_H
#define CAMERA_H

class Camera {
public:
  Camera();
  Camera(Camera &&) = delete;
  Camera(const Camera &) = delete;
  Camera &operator=(Camera &&) = default;
  Camera &operator=(const Camera &) = delete;
  ~Camera();

private:
};

#endif // !CAMERA_H
