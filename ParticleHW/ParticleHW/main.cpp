#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <SFML/System.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/Main.hpp>
#include <math.h>

using namespace sf;
using namespace std;

int resizeCounter = 0;
int resizeCounter2 = 0;
float disToAttractor;
float xDir;
float yDir;
float disConstant;
float attractorRadius = 175;

class ParticleSystem : public sf::Drawable, public sf::Transformable
{
public:

	ParticleSystem(unsigned int count) :
		m_particles(count),
		m_vertices(sf::Lines, count),
		m_lifetime(sf::seconds(3)),
		m_emitter(250, 50),
		m_emitter2(150, 150),
		m_attractor(0, 0)


	{
		if (!texture.loadFromFile("3.png"))
			printf("error");
		else
			printf("loaded");
	}

	void setAttractor(Vector2f position) {
		m_attractor = position;
	}

	void setEmitter(sf::Vector2f position)
	{
		m_emitter = position;
		m_emitter2 = position;
	}

	void update(sf::Time elapsed)
	{
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
			m_particles.resize(m_particles.size() + 5);
			//m_vertices.resize(m_particles.size());
			resizeCounter++;
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && resizeCounter) {
			m_particles.resize(m_particles.size() - 5);
			//m_vertices.resize(m_particles.size());
			resizeCounter--;
		}
		
		for (std::size_t i = 0; i < m_particles.size(); ++i)
		{
			// update the particle lifetime
			Particle& p = m_particles[i];
			p.lifetime -= elapsed;

			// if the particle is dead, respawn it
			if (p.lifetime <= sf::Time::Zero)
				resetParticle(i);

			// update the position of the corresponding vertex
			//p.velocity.x += 0;
			//p.velocity.y += 1 * elapsed.asSeconds();

			p.velocity.y += 1;

			//get distance to attractor
			//based on position of one vertex of quads, assign direction
			//increase the force based on distance (closer == higher)
			//if in attractor radius, apply the force in x, and y
			disToAttractor = sqrt(pow(p.vrtxArr[3].position.x - m_attractor.x, 2) + pow(p.vrtxArr[3].position.y - m_attractor.y, 2));
			
			if (p.vrtxArr[3].position.x > m_attractor.x)
				xDir = -1;
			else
				xDir = 1;

			if (p.vrtxArr[3].position.y > m_attractor.y)
				yDir = -1;
			else
				yDir = 1;

			if (disToAttractor < attractorRadius) {
				disConstant = ((attractorRadius - disToAttractor) / (pow(attractorRadius, 1))) * 5;
				p.velocity.x += xDir * disConstant;
				p.velocity.y += yDir * disConstant;
			}


			//m_vertices[i].position += p.velocity * elapsed.asSeconds();
			p.vrtxArr[0].position += p.velocity * elapsed.asSeconds();
			p.vrtxArr[1].position += p.velocity * elapsed.asSeconds();
			p.vrtxArr[2].position += p.velocity * elapsed.asSeconds();
			p.vrtxArr[3].position += p.velocity * elapsed.asSeconds();

			p.vrtxArr[0].texCoords =  Vector2f(400, 400);
			p.vrtxArr[1].texCoords =  Vector2f(400, 0);
			p.vrtxArr[2].texCoords =  Vector2f(0, 0);
			p.vrtxArr[3].texCoords =  Vector2f(0, 400);




			//p.vrtxArr[0].texCoords += p.velocity * elapsed.asSeconds();
			//p.vrtxArr[1].texCoords += p.velocity * elapsed.asSeconds();
			//p.vrtxArr[2].texCoords += p.velocity * elapsed.asSeconds();
			//p.vrtxArr[3].texCoords += p.velocity * elapsed.asSeconds();
			

			// update the alpha (transparency) of the particle according to its lifetime
			float ratio = p.lifetime.asSeconds() / m_lifetime.asSeconds();
			//m_vertices[i].color.a = static_cast<sf::Uint8>(ratio * 255);
		}
	}

private:

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		// apply the transform
		states.transform *= getTransform();

		// our particles don't use a texture
		states.texture = &texture;

		// draw the vertex array
		target.draw(m_vertices, states);
		for (int i = 0; i < m_particles.size(); i++) {
			target.draw(m_particles[i].vrtxArr, states);
		}
	}

private:

	struct Particle
	{
		sf::Vector2f velocity;
		sf::Time lifetime;
		VertexArray vrtxArr;

	};

	void resetParticle(std::size_t index)
	{
		// give a random velocity and lifetime to the particle
		float angle = (std::rand() % 90) * 3.14f / 180.f;
		float speed = (std::rand() % 50) + 50.f;
		m_particles[index].velocity = sf::Vector2f(std::cos(angle) * speed, std::sin(angle) * speed);
		m_particles[index].lifetime = sf::milliseconds((std::rand() % 2000) + 1000);

		// reset the position of the corresponding vertex
		if (index % 5) {
			m_particles[index].vrtxArr = VertexArray(Quads, 4);
			m_particles[index].vrtxArr[0] = m_emitter + Vector2f(4, 4);
			m_particles[index].vrtxArr[1] = m_emitter + Vector2f(4, -4);
			m_particles[index].vrtxArr[2] = m_emitter + Vector2f(-4, -4);
			m_particles[index].vrtxArr[3] = m_emitter + Vector2f(-4, 4);

			/*m_particles[index].vrtxArr[0].texCoords = m_emitter + Vector2f(10, 10);
			m_particles[index].vrtxArr[1].texCoords = m_emitter + Vector2f(10, -10);
			m_particles[index].vrtxArr[2].texCoords = m_emitter + Vector2f(-10, -10);
			m_particles[index].vrtxArr[3].texCoords = m_emitter + Vector2f(-10, 10);*/

			//m_particles[index].vrtxArr[0].texCoords += m_particles[index].velocity ;
			//m_particles[index].vrtxArr[1].texCoords += m_particles[index].velocity ;
			//m_particles[index].vrtxArr[2].texCoords += m_particles[index].velocity ;
			//m_particles[index].vrtxArr[3].texCoords += m_particles[index].velocity ;

		}
			//m_vertices[index].position = m_emitter;
		else {
			m_particles[index].vrtxArr = VertexArray(Quads, 4);
			m_particles[index].vrtxArr[0] = m_emitter2 + Vector2f(4, 4);
			m_particles[index].vrtxArr[1] = m_emitter2 + Vector2f(4, -4);
			m_particles[index].vrtxArr[2] = m_emitter2 + Vector2f(-4, -4);
			m_particles[index].vrtxArr[3] = m_emitter2 + Vector2f(-4, 4);
		}
			//m_vertices[index].position = m_emitter2;

		//m_particles[index].vrtxArr = VertexArray(Quads, 4);
		//m_particles[index].vrtxArr[0] = m_emitter + Vector2f(5, 5);
		//m_particles[index].vrtxArr[1] = m_emitter + Vector2f(-5, 5);
		//m_particles[index].vrtxArr[2] = m_emitter + Vector2f(5, -5);
		//m_particles[index].vrtxArr[3] = m_emitter + Vector2f(-5, -5);

	}

	std::vector<Particle> m_particles;
	sf::VertexArray m_vertices;
	sf::Time m_lifetime;
	sf::Vector2f m_emitter;
	sf::Vector2f m_emitter2;
	sf::Texture texture;
	sf::Vector2f m_attractor;

};

class ParticleSystem2 : public sf::Drawable, public sf::Transformable
{
public:

	ParticleSystem2(unsigned int count) :
		m_particles(count),
		//m_vertices(sf::Points, count),
		m_lifetime(sf::seconds(3)),
		m_emitter(370, 151),
		m_emitter2(50, 50),
		m_attractor(0,0)


	{
		if (!texture.loadFromFile("5.jpg"))
			printf("error");
		else
			printf("loaded");
	}

	void setEmitter(sf::Vector2f position)
	{
		m_emitter = position;
		m_emitter2 = position;
	}

	void setAttractor(Vector2f position) {
		m_attractor = position;
	}

	void update(sf::Time elapsed)
	{
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
			m_particles.resize(m_particles.size() + 100);
			m_vertices.resize(m_particles.size());
			resizeCounter2++;
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && resizeCounter2) {
			m_particles.resize(m_particles.size() - 100);
			m_vertices.resize(m_particles.size());
			resizeCounter2--;
		}

		for (std::size_t i = 0; i < m_particles.size(); ++i)
		{
			// update the particle lifetime
			Particle& p = m_particles[i];
			p.lifetime -= elapsed;

			// if the particle is dead, respawn it
			if (p.lifetime <= sf::Time::Zero)
				resetParticle(i);

			// update the position of the corresponding vertex
			//p.velocity.x += 0;
			p.velocity.y -= 1;

			//m_vertices[i].position += p.velocity * elapsed.asSeconds();
			p.vrtxArr[0].position += p.velocity * elapsed.asSeconds();
			p.vrtxArr[1].position += p.velocity * elapsed.asSeconds();
			p.vrtxArr[2].position += p.velocity * elapsed.asSeconds();
			p.vrtxArr[3].position += p.velocity * elapsed.asSeconds();

			p.vrtxArr[0].texCoords = Vector2f(400, 400);
			p.vrtxArr[1].texCoords = Vector2f(400, 0);
			p.vrtxArr[2].texCoords = Vector2f(0, 0);
			p.vrtxArr[3].texCoords = Vector2f(0, 400);

			//p.vrtxArr[0].texCoords += p.velocity * elapsed.asSeconds();
			//p.vrtxArr[1].texCoords += p.velocity * elapsed.asSeconds();
			//p.vrtxArr[2].texCoords += p.velocity * elapsed.asSeconds();
			//p.vrtxArr[3].texCoords += p.velocity * elapsed.asSeconds();


			// update the alpha (transparency) of the particle according to its lifetime
			float ratio = p.lifetime.asSeconds() / m_lifetime.asSeconds();
			//m_vertices[i].color.a = static_cast<sf::Uint8>(ratio * 255);
		}
	}

private:

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		// apply the transform
		states.transform *= getTransform();

		// our particles don't use a texture
		states.texture = &texture;

		// draw the vertex array
		target.draw(m_vertices, states);
		for (int i = 0; i < m_particles.size(); i++) {
			target.draw(m_particles[i].vrtxArr, states);
		}
	}

private:

	struct Particle
	{
		sf::Vector2f velocity;
		sf::Time lifetime;
		VertexArray vrtxArr;
	};

	void resetParticle(std::size_t index)
	{
		// give a random velocity and lifetime to the particle
		float angle = (std::rand() % 90) * 3.14f / 180.f;
		float speed = (std::rand() % 50) + 20.f;
		m_particles[index].velocity = sf::Vector2f(std::cos(angle) * speed, std::sin(angle) * speed);
		m_particles[index].lifetime = sf::milliseconds((std::rand() % 2000) + 1000);

		// reset the position of the corresponding vertex
		if (!(index % 5)) {
			m_particles[index].vrtxArr = VertexArray(Quads, 4);
			m_particles[index].vrtxArr[0] = m_emitter + Vector2f(1, 1);
			m_particles[index].vrtxArr[1] = m_emitter + Vector2f(1, -1);
			m_particles[index].vrtxArr[2] = m_emitter + Vector2f(-1, -1);
			m_particles[index].vrtxArr[3] = m_emitter + Vector2f(-1, 1);

			/*m_particles[index].vrtxArr[0].texCoords = m_emitter + Vector2f(10, 10);
			m_particles[index].vrtxArr[1].texCoords = m_emitter + Vector2f(10, -10);
			m_particles[index].vrtxArr[2].texCoords = m_emitter + Vector2f(-10, -10);
			m_particles[index].vrtxArr[3].texCoords = m_emitter + Vector2f(-10, 10);*/

			//m_particles[index].vrtxArr[0].texCoords += m_particles[index].velocity ;
			//m_particles[index].vrtxArr[1].texCoords += m_particles[index].velocity ;
			//m_particles[index].vrtxArr[2].texCoords += m_particles[index].velocity ;
			//m_particles[index].vrtxArr[3].texCoords += m_particles[index].velocity ;

		}
		//m_vertices[index].position = m_emitter;
		else {
			m_particles[index].vrtxArr = VertexArray(Quads, 4);
			m_particles[index].vrtxArr[0] = m_emitter2 + Vector2f(1, 1);
			m_particles[index].vrtxArr[1] = m_emitter2 + Vector2f(1, -1);
			m_particles[index].vrtxArr[2] = m_emitter2 + Vector2f(-1, -1);
			m_particles[index].vrtxArr[3] = m_emitter2 + Vector2f(-1, 1);
		}
		//m_vertices[index].position = m_emitter2;

		//m_particles[index].vrtxArr = VertexArray(Quads, 4);
		//m_particles[index].vrtxArr[0] = m_emitter + Vector2f(5, 5);
		//m_particles[index].vrtxArr[1] = m_emitter + Vector2f(-5, 5);
		//m_particles[index].vrtxArr[2] = m_emitter + Vector2f(5, -5);
		//m_particles[index].vrtxArr[3] = m_emitter + Vector2f(-5, -5);

	}

	std::vector<Particle> m_particles;
	sf::VertexArray m_vertices;
	sf::Time m_lifetime;
	sf::Vector2f m_emitter;
	sf::Vector2f m_emitter2;
	sf::Texture texture;
	sf::Vector2f m_attractor;

};


int main()
{
	// Load a texture from a file


	// create the window
	sf::RenderWindow window(sf::VideoMode(512, 256), "Particles");

	// create the particle system
	ParticleSystem particles(100);
	ParticleSystem2 particles2(1000);


	// create a clock to track the elapsed time
	sf::Clock clock;

	// run the main loop
	while (window.isOpen())
	{
		// handle events
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		// make the particle system emitter follow the mouse
		sf::Vector2i mouse = sf::Mouse::getPosition(window);
		particles.setAttractor(window.mapPixelToCoords(mouse));

		// update it
		sf::Time elapsed = clock.restart();
		particles.update(elapsed);
		particles2.update(elapsed);


		// draw it
		window.clear();
		window.draw(particles);
		window.draw(particles2);
		window.display();
	}

	return 0;
}