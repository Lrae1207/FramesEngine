#include "SFML/Audio.hpp"
#include "SFML/System.hpp"
#include "SFML/Graphics.hpp"
#include "SFML/Window.hpp"
#include "SFML/Network.hpp"

#include "Macros.hpp"

#ifdef ENGINE_EXPORTS
	#define ENGINE_API __declspec(dllexport)
#else
	#define ENGINE_API __declspec(dllimport)
#endif

// Forward declaration of all classes in the engine to stop the compiler from screaming
namespace engine {
	struct ASTARNode;
	class ASTARGrid;
	class AnimationNode;
	class Animator;
	class Controller;
	class Transform;
	class AudioManager;
	class Camera;
	class PhysicsSettings;
	struct engine_settings;
	class GameObject;
	class Game;
	struct Force;
	struct StrippedTransform;
	class Renderable;
	struct Rect;
	class Line;
	class Text;
	class Shape;
	class Texture;
	class LoadableTexture;
	class PerlinNoise;

	namespace filesystem {
		class Parser;
	}

	namespace collisions {
		class Ray;
		class Face;
		class Collider;
		class CollisionPair;
		class ColliderBound;
		struct Normal;
		class CollisionManager;
		class PolygonCollider;
	}
};