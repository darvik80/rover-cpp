import React from 'react';
import { Alert } from 'reactstrap';

const errToArray = (err) => {
  if (!err.response || !err.response.data) return [err];
  const resp = err.response;
  if (typeof resp.data === 'string') return [resp.data];
  if (Array.isArray(resp.data.errors)) return resp.data.errors.filter(err => typeof err === 'string');
  return [resp.data.toString()];
};

/**
 * @description Component to catch error in children and render error or Fallback UI
 * @see get more info here: https://reactjs.org/docs/error-boundaries.html
 */
class ErrorBoundary extends React.Component {
  constructor(props) {
    super(props);
    this.state  = { hasError: false, error: null, info: null };
    this.toggle = this.toggle.bind(this);
  }

  componentDidCatch(error, info) {
    this.setState({
      hasError: true,
      error   : error,
      info    : info,
    });

    // You can also log the error to an error reporting service
    // TODO: send to server
    console.log(error, info);
  }

  toggle() {
    this.setState({ hasError: false });
  }

  render() {
    if (!this.state.hasError) {
      if (!this.props.children) return null;
      return this.props.children;
    }

    const errMsg = errToArray(this.state.error).join('\n');

    // Fallback UI
    return (
      <Alert color="danger" isOpen={this.state.isOpen} toggle={this.toggle}>
        <h5>Oops, something went wrong :( &nbsp;&nbsp;&nbsp; You can close this message to retry</h5>
        <pre style={{ whiteSpace: 'pre-wrap' }}>
            The error: {errMsg}
          </pre>
        <pre style={{ whiteSpace: 'pre-wrap' }}>
            Where it occured: {this.state.info ? this.state.info.componentStack : ''}
          </pre>
      </Alert>
    );
  }
}

export default ErrorBoundary;
