import { PageContextProvider } from './usePageContext';
import './PageShell.css';

export { PageShell };

function PageShell({ pageContext, children }) {
	return (
		<PageContextProvider pageContext={pageContext}>
			<Header url={pageContext.urlPathname} />
			<main>{children}</main>
		</PageContextProvider>
	);
}

export function Header({ url }) {
	return (
		<header>
			<nav>
				<a href="/" class={url == '/' && 'active'}>
					Home
				</a>
				<a href="/config" class={url == '/config' && 'active'}>
					Device Configuration
				</a>
				<a href="/data" class={url == '/data' && 'active'}>
					Device Data
				</a>
			</nav>
		</header>
	);
}
